#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>

namespace yamanix {
// Project-owned bounded memory routing;facts and policy:docs/an1x-memory-map.md.
// No peripheral/MMIO fallback,no ROM transformations,no physical reset claims.
class An1xMemoryMap {
public:
    static constexpr uint32_t address_mask=0xffffff;
    static constexpr std::size_t rom_size=0x100000,nvram_size=0x40000,internal_size=0x200;
    enum class Region {rom,nvram,internal_ram,unmapped};
    explicit An1xMemoryMap(std::span<const uint8_t> rom)
        :rom_(checked_rom(rom)),nvram_(nvram_size),internal_(internal_size){}
    An1xMemoryMap(const An1xMemoryMap&)=delete;
    An1xMemoryMap& operator=(const An1xMemoryMap&)=delete;
    An1xMemoryMap(An1xMemoryMap&&)=delete;
    An1xMemoryMap& operator=(An1xMemoryMap&&)=delete;
    static Region region(uint32_t raw) {
        const auto a=raw&address_mask;
        if(a<rom_size)return Region::rom;
        if(a>=0x200000&&a<=0x23ffff)return Region::nvram;
        if(a>=0xfffd10&&a<=0xffff0f)return Region::internal_ram;
        return Region::unmapped;
    }
    uint8_t read_byte(uint32_t raw)const {
        const auto a=raw&address_mask;
        switch(region(a)) {
        case Region::rom:return rom_[a];
        case Region::nvram:return nvram_[a-0x200000];
        case Region::internal_ram:return internal_[a-0xfffd10];
        case Region::unmapped:throw std::out_of_range("Unmapped AN1x byte read");
        }
        throw std::logic_error("Invalid AN1x memory region");
    }
    void write_byte(uint32_t raw,uint8_t value) {
        const auto a=raw&address_mask;
        switch(region(a)) {
        case Region::nvram:nvram_[a-0x200000]=value;return;
        case Region::internal_ram:internal_[a-0xfffd10]=value;return;
        case Region::rom:throw std::runtime_error("AN1x ROM write rejected");
        case Region::unmapped:throw std::out_of_range("Unmapped AN1x byte write");
        }
        throw std::logic_error("Invalid AN1x memory region");
    }
    uint16_t read_word(uint32_t raw)const {
        require_word(raw,false);
        return uint16_t((uint16_t(read_byte(raw))<<8)|read_byte(raw+1));
    }
    uint16_t read_instruction_word(uint32_t raw)const{return read_word(raw);}
    void write_word(uint32_t raw,uint16_t value) {
        require_word(raw,true); // Reject unsupported pair before any mutation.
        write_byte(raw,uint8_t(value>>8));write_byte(raw+1,uint8_t(value));
    }
    // Caller-owned persistence;no file I/O and no reinterpretation of inputs.
    std::vector<uint8_t> nvram_image()const{return nvram_;}
    void load_nvram(std::span<const uint8_t> image) {
        if(image.size()!=nvram_size)throw std::invalid_argument("AN1x NVRAM size");
        std::vector<uint8_t> candidate(image.begin(),image.end());nvram_.swap(candidate);
    }
    // Explicit deterministic research operation,not a hardware reset model.
    void clear_internal_ram(){std::fill(internal_.begin(),internal_.end(),uint8_t(0));}
private:
    static std::vector<uint8_t> checked_rom(std::span<const uint8_t> rom) {
        if(rom.size()!=rom_size)throw std::invalid_argument("AN1x logical ROM size");
        return {rom.begin(),rom.end()};
    }
    static void require_word(uint32_t raw,bool write) {
        if(raw&1)throw std::invalid_argument("Unaligned AN1x word access");
        for(unsigned i=0;i<2;++i){const auto r=region(raw+i);
            if(r==Region::unmapped)throw std::out_of_range("Unmapped AN1x word access");
            if(write&&r==Region::rom)throw std::runtime_error("AN1x ROM word write rejected");
        }
    }
    std::vector<uint8_t> rom_,nvram_,internal_;
};
} // namespace yamanix
