#pragma once
#include "an1x_memory_map.h"
#include "h8_digital_port.h"
#include "sci_cycle_timeline.h"
#include <utility>
#include <type_traits>

namespace yamanix {
// Project-owned bounded composition;docs/an1x-sci-bus.md.
// Sink must outlive this nonmovable host;callbacks must not reenter or mutate it.
// Sink:tx(channel,cycle,level),clock(channel,cycle,level),dma_request(vector,cycle).
// GPIO access requires gpio_read(port,cycle)/gpio_write(port,cycle,data,direction).
// Optional exception_hook(vector). No real-time threading/reentry support.
template<class Sink>class An1xSciBus {
public:
    using RxEvent=SciCycleTimeline::RxEvent;
    An1xSciBus(std::span<const uint8_t> rom,std::span<const RxEvent> input,Sink& sink,
              std::size_t event_limit=65536)
        :memory_(rom),sci_(irq_),input_(input.begin(),input.end()),timeline_(sci_,input_),sink_(sink),limit_(event_limit){}
    An1xSciBus(const An1xSciBus&)=delete;
    An1xSciBus& operator=(const An1xSciBus&)=delete;
    H8hIrqController& irq(){return irq_;}
    bool failed()const{return failed_;}
    uint64_t cycle()const{return timeline_.cycle();}
    std::size_t consumed_input()const{return timeline_.consumed_input();}
    std::vector<uint8_t> nvram_image()const{return memory_.nvram_image();}
    void load_nvram(std::span<const uint8_t> image){access([&]{memory_.load_nvram(image);});}
    uint8_t peek_sci(uint32_t a){return access([&]{return sci_.read8(a&0xffffff,false);});}
    uint64_t update(uint64_t cycle){return access([&]{advance(cycle);return deadline();});}
    void bus_cycle(uint64_t cycle){access([&]{advance(cycle);now_=timeline_.cycle();});}
    uint64_t next_bus_event()const{return deadline();}
    uint16_t read_instruction_word(uint32_t a){return access([&]{return memory_.read_instruction_word(a);});}
    uint8_t read_byte(uint32_t raw){return access([&]{const auto a=raw&0xffffff;return H83002SciMmio::mapped(a)?sci_.read8(a):gpio_mapped(a)?read_gpio(a):memory_.read_byte(a);});}
    void write_byte(uint32_t raw,uint8_t v){access([&]{const auto a=raw&0xffffff;if(H83002SciMmio::mapped(a))sci_.write8(a,v,now_,*this);else if(gpio_mapped(a))write_gpio(a,v);else memory_.write_byte(a,v);});}
    uint16_t read_word(uint32_t raw){return access([&]{const auto a=raw&0xffffff;return H83002SciMmio::mapped(a)?sci_.read16(a):memory_.read_word(a);});}
    void write_word(uint32_t raw,uint16_t v){access([&]{const auto a=raw&0xffffff;if(H83002SciMmio::mapped(a))sci_.write16(a,v,0xffff,now_,*this);else memory_.write_word(a,v);});}
    void exception_hook(int v){access([&]{if constexpr(requires{sink_.exception_hook(v);})sink_.exception_hook(v);});}
    // Timed timeline sink and untimed MMIO sink,sharing one outward clock.
    void tx(unsigned c,uint64_t at,bool v){sink_.tx(c,at,v);}
    void clock(unsigned c,uint64_t at,bool v){sink_.clock(c,at,v);}
    bool dma_request(unsigned v,uint64_t at){return sink_.dma_request(v,at);}
    void tx(unsigned c,bool v){tx(c,now_,v);}
    void clock(unsigned c,bool v){clock(c,now_,v);}
    bool dma_request(unsigned v){return dma_request(v,now_);}
private:
    template<class F>decltype(auto) access(F&& f){
        if(busy_||failed_){failed_=true;throw std::logic_error("Unavailable AN1x SCI bus");}
        busy_=true;
        try{
            if constexpr(std::is_void_v<std::invoke_result_t<F>>){std::forward<F>(f)();if(failed_)throw std::logic_error("Reentered AN1x SCI bus");busy_=false;return;}
            else {auto value=std::forward<F>(f)();if(failed_)throw std::logic_error("Reentered AN1x SCI bus");busy_=false;return value;}
        }catch(...){failed_=true;busy_=false;throw;}
    }
    static bool gpio_mapped(uint32_t a){return a==0xffffc5||a==0xffffc7||a==0xffffda||a==0xffffc9||a==0xffffcb||a==0xffffcd||a==0xffffcf||a==0xffffce||a==0xffffd0||a==0xffffd2||a==0xffffd1||a==0xffffd3||a==0xffffd4||a==0xffffd6;}
    uint8_t read_gpio(uint32_t a){
        if(a==0xffffc5||a==0xffffc9||a==0xffffcd||a==0xffffd0||a==0xffffd1||a==0xffffd4)return 0xff; // H83002 DDR map uses ff_r.
        if(a==0xffffda)return port4_.pcr();
        const unsigned id=a==0xffffd6?11u:a==0xffffd3?10u:a==0xffffd2?9u:a==0xffffce?7u:a==0xffffcf?8u:a==0xffffcb?6u:4u;
        auto& port=id==11?portb_:id==10?porta_:id==9?port9_:id==7?port7_:id==8?port8_:id==6?port6_:port4_;
        return port.read_pins([&]() -> uint8_t {
            if constexpr(requires{sink_.gpio_read(id,now_);})return sink_.gpio_read(id,now_);
            else throw std::logic_error("GPIO input sink unavailable");
        });
    }
    void write_gpio(uint32_t a,uint8_t value){
        if(a==0xffffce)throw std::logic_error("Read-only AN1x port7");
        if(a==0xffffda){port4_.write_pcr(value);return;}
        const unsigned id=(a==0xffffd4||a==0xffffd6)?11u:(a==0xffffd1||a==0xffffd3)?10u:(a==0xffffd0||a==0xffffd2)?9u:(a==0xffffcd||a==0xffffcf)?8u:(a==0xffffc9||a==0xffffcb)?6u:4u;
        auto& port=id==11?portb_:id==10?porta_:id==9?port9_:id==8?port8_:id==6?port6_:port4_;
        auto output=[&](uint8_t data,uint8_t direction){
            if constexpr(requires{sink_.gpio_write(id,now_,data,direction);})sink_.gpio_write(id,now_,data,direction);
            else throw std::logic_error("GPIO output sink unavailable");
        };
        if(a==0xffffc5||a==0xffffc9||a==0xffffcd||a==0xffffd0||a==0xffffd1||a==0xffffd4)port.write_ddr(value,output);else port.write_data(value,output);
    }
    uint64_t deadline()const{
        auto next=sci_.next_event();const auto i=timeline_.consumed_input();
        if(i<input_.size()&&(!next||input_[i].cycle<next))next=input_[i].cycle;
        return next;
    }
    void advance(uint64_t target){
        // A completed CPU bus access may overrun its slice. Its effects stay
        // committed while the next slice repays debt;do not rewind peripherals.
        target=std::max(target,timeline_.cycle());
        if(!timeline_.advance_to(target,limit_,*this).complete)
            throw std::runtime_error("AN1x SCI event budget exhausted");
        now_=timeline_.cycle();
    }
    An1xMemoryMap memory_;
    H8DigitalPort port4_;
    H8DigitalPort port6_{0x80,0x80};
    H8DigitalPort port8_{0xf0,0xe0};
    H8DigitalPort port7_;
    H8DigitalPort port9_{0x00,0xc0};
    H8DigitalPort porta_;
    H8DigitalPort portb_;
    H8hIrqController irq_;
    H83002SciMmio sci_;
    const std::vector<RxEvent> input_;
    SciCycleTimeline timeline_;
    Sink& sink_;
    std::size_t limit_;
    uint64_t now_=0;
    bool busy_=false,failed_=false;
};
} // namespace yamanix
