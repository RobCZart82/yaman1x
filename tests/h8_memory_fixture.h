#pragma once
#include <array>
#include <cstdint>
#include <map>
#include <tuple>
#include <vector>
#include <stdexcept>
// Synthetic split instruction/data host; no Yamaha inputs or real MMIO map.
struct Host {
 std::array<uint16_t,512> words{};
 std::map<uint32_t,uint8_t> data;
 std::vector<std::tuple<char,uint32_t,uint16_t>> trace;
 uint64_t event=11;bool bad_read=false,bad_write=false;unsigned word_writes=0,fail_word_write=0;
 Host(){words[1]=0x100;}
 void program(const std::vector<uint16_t>& code){for(unsigned i=0;i<code.size();++i)words[0x80+i]=code[i];}
 uint16_t read_instruction_word(uint32_t address){if((address&1)||address/2>=words.size())throw std::out_of_range("instruction bus");auto v=words[address/2];trace.emplace_back('I',address,v);return v;}
 uint8_t read_byte(uint32_t address){if(bad_read)throw std::runtime_error("data read");auto v=data[address&0xffffff];trace.emplace_back('R',address,v);return v;}
 void write_byte(uint32_t address,uint8_t value){data[address&0xffffff]=value;trace.emplace_back('W',address,value);if(bad_write)throw std::runtime_error("data write after mutation");}
 uint16_t read_word(uint32_t address){if(bad_read)throw std::runtime_error("data word read");auto v=uint16_t((uint16_t(data[address&0xffffff])<<8)|data[(address+1)&0xffffff]);trace.emplace_back('Q',address,v);return v;}
 void write_word(uint32_t address,uint16_t value){data[address&0xffffff]=uint8_t(value>>8);data[(address+1)&0xffffff]=uint8_t(value);trace.emplace_back('S',address,value);++word_writes;if(bad_write || (fail_word_write && word_writes==fail_word_write))throw std::runtime_error("data word write");}
 uint64_t update(uint64_t cycle){while(event<=cycle)event+=11;return event;}
};
