#include <sstream>
#include <limits>
#include "coder.hpp"
#include "error.hpp"

namespace hackasm
{

const std::unordered_map<const char*, uint16_t> coder::comp_codes = {
  {"0"  , 0b0101010},  
  {"1"  , 0b0111111},  
  {"-1" , 0b0111010},  
  {"D"  , 0b0001100},  
  {"A"  , 0b0110000},  
  {"!D" , 0b0001101},  
  {"!A" , 0b0110001},  
  {"-D" , 0b0001111},  
  {"-A" , 0b0110011},  
  {"D+1", 0b0011111},  
  {"A+1", 0b0110111},  
  {"D-1", 0b0001110},  
  {"A-1", 0b0110010},  
  {"D+A", 0b0000010},  
  {"D-A", 0b0010011},  
  {"A-D", 0b0000111},  
  {"D&A", 0b0000000},  
  {"D|A", 0b0010101},  
  {"M"  , 0b1110000},  
  {"!M" , 0b1110001},  
  {"-M" , 0b1110011},  
  {"M+1", 0b1110111},  
  {"M-1", 0b1110010},  
  {"D+M", 0b1000010},  
  {"D-M", 0b1010011},  
  {"M-D", 0b1000111},  
  {"D&M", 0b1000000},  
  {"D|M", 0b1010101},  
};
const std::unordered_map<const char*, uint16_t> coder::dest_codes = {
  {"M"  , 0b001},  
  {"D"  , 0b010},  
  {"DM" , 0b011},  
  {"A"  , 0b100},  
  {"AM" , 0b101},  
  {"AD" , 0b110},  
  {"ADM", 0b111},  
};
const std::unordered_map<const char*, uint16_t> coder::jump_codes = {
  {"JGT", 0b001},  
  {"JEQ", 0b010},  
  {"JGE", 0b011},  
  {"JLT", 0b100},  
  {"JNE", 0b101},  
  {"JLE", 0b110},  
  {"JMP", 0b111},  
};

const std::unordered_map<const char*, hack_address> coder::common_symbol_table = {
  {"R0"    , 0    },  
  {"R1"    , 1    },  
  {"R2"    , 2    },  
  {"R3"    , 3    },  
  {"R4"    , 4    },  
  {"R5"    , 5    },  
  {"R6"    , 6    },  
  {"R7"    , 7    },  
  {"R8"    , 8    },  
  {"R9"    , 9    },  
  {"R10"   , 10   },  
  {"R11"   , 11   },  
  {"R12"   , 12   },  
  {"R13"   , 13   },  
  {"R14"   , 14   },  
  {"R15"   , 15   },  
  {"SP"    , 0    },  
  {"LCL"   , 1    },  
  {"ARG"   , 2    },  
  {"THIS"  , 3    },  
  {"THAT"  , 4    },  
  {"SCREEN", 16384},  
  {"KBD"   , 24576},  
};

static const hack_address bad_address = std::numeric_limits<hack_address>::max();

coder::coder() 
{
  for(const auto& pair : coder::common_symbol_table) {
    _symbol_table.emplace(std::string{pair.first}, pair.second); 
  }
}

void coder::declare_label(const std::string& label, hack_address address)
{
  auto search_address = find_symbol_address(label);
  if(search_address != bad_address) {
    throw multiple_label_declarations{label, search_address, address};
  }
  _symbol_table.emplace(label, address);
}

bin_cmd coder::code(const asm_cmd& cmd)
{

}

hack_address coder::find_symbol_address(const std::string& symbol)
{
  auto iter = _symbol_table.find(symbol);
  return (iter != _symbol_table.end()) ? iter->second : bad_address;
}

} // namespace hackasm

