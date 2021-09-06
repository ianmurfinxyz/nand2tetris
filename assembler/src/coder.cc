#include <sstream>
#include <cassert>
#include <limits>
#include "coder.hh"
#include "error.hh"

namespace hackasm
{
  const std::unordered_map<std::string, uint16_t> coder::comp_codes = {
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
  const std::unordered_map<std::string, uint16_t> coder::dest_codes = {
    {"M"  , 0b001},
    {"D"  , 0b010},
    {"DM" , 0b011},
    {"A"  , 0b100},
    {"AM" , 0b101},
    {"AD" , 0b110},
    {"ADM", 0b111},
  };
  const std::unordered_map<std::string, uint16_t> coder::jump_codes = {
    {"JGT", 0b001},
    {"JEQ", 0b010},
    {"JGE", 0b011},
    {"JLT", 0b100},
    {"JNE", 0b101},
    {"JLE", 0b110},
    {"JMP", 0b111},
  };
  const std::unordered_map<std::string, hack_address> coder::base_symbols = {
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

  coder::coder() : _next_ram_adress{static_cast<hack_address>(base_symbols.size())}
  {
    assert(initial_ram_address == static_cast<hack_address>(base_symbols.size()));
    for(const auto [symbol, address] : coder::base_symbols) {
      _symbol_table.emplace(std::string{symbol}, address);
    }
  }

  void coder::add_label(const std::string& symbol, hack_address address)
  {
    if(_symbol_table.contains(symbol)){
      throw duplicate_label{symbol};
    }
    _symbol_table.emplace(symbol, address);
  }

  bin_cmd coder::code(const asm_cmd& cmd)
  {
    switch(cmd._type){
      case cmd_type::A_CMD:
        return code_a_cmd(cmd);
      case cmd_type::C0_CMD:
        return code_c0_cmd(cmd);
      case cmd_type::C1_CMD:
        return code_c1_cmd(cmd);
      case cmd_type::C2_CMD:
        return code_c2_cmd(cmd);
      default:
        assert(0);
    }
  }

  bin_cmd coder::code_a_cmd(const asm_cmd &cmd)
  {
    hack_address address = 0;
    if(cmd._symbol.length() > 0){
      auto result = _symbol_table.find(cmd._symbol);
      if(result == _symbol_table.end()){
        _symbol_table.emplace(cmd._symbol, _next_ram_adress);
        address = _next_ram_adress++;
      }
      else {
        address = result->second;
      }
    }
    else if(cmd._address.length() > 0){
      long long raw_address = 0;
      raw_address = std::stoi(cmd._address);
      if(raw_address > std::numeric_limits<hack_address>::max()){
        throw hack_address_out_of_range{raw_address};
      }
      address = static_cast<hack_address>(raw_address);
    }
    else {
      assert(0);
    }
    return address;
  }

  bin_cmd coder::code_c0_cmd(const asm_cmd &cmd)
  {
    auto dest_code = get_dest_code(cmd._dest);
    auto comp_code = get_comp_code(cmd._comp);
    auto jump_code = get_jump_code(cmd._jump);
    if(references_m(cmd._dest) || references_m(cmd._comp)){
      throw a_register_conflict{cmd._dest + "=" + cmd._comp + ";" + cmd._jump};
    }
    bin_cmd bin = 0;
    bin |= 0b111 << 13;
    bin |= dest_code << 3;
    bin |= comp_code << 6;
    bin |= jump_code;
    return bin;
  }

  bin_cmd coder::code_c1_cmd(const asm_cmd &cmd)
  {
    auto dest_code = get_dest_code(cmd._dest);
    auto comp_code = get_comp_code(cmd._comp);
    bin_cmd bin = 0;
    bin |= 0b111 << 13;
    bin |= dest_code << 3;
    bin |= comp_code << 6;
    return bin;
  }

  bin_cmd coder::code_c2_cmd(const asm_cmd &cmd)
  {
    auto comp_code = get_comp_code(cmd._comp);
    auto jump_code = get_jump_code(cmd._jump);
    if(references_m(cmd._comp)){
      throw a_register_conflict{cmd._comp + ";" + cmd._jump};
    }
    bin_cmd bin = 0;
    bin |= 0b111 << 13;
    bin |= comp_code << 6;
    bin |= jump_code;
    return bin;
  }

  uint16_t coder::get_dest_code(const std::string& mnemonic)
  {
    auto result = dest_codes.find(mnemonic.c_str());
    if(result == dest_codes.end()){
      throw unknown_mnemonic{"dest", mnemonic};
    }
    return result->second;
  }

  uint16_t coder::get_comp_code(const std::string& mnemonic)
  {
    auto result = comp_codes.find(mnemonic.c_str());
    if(result == comp_codes.end()){
      throw unknown_mnemonic{"comp", mnemonic};
    }
    return result->second;
  }

  uint16_t coder::get_jump_code(const std::string& mnemonic)
  {
    auto result = jump_codes.find(mnemonic.c_str());
    if(result == jump_codes.end()){
      throw unknown_mnemonic{"jump", mnemonic};
    }
    return result->second;
  }

  bool coder::references_m(const std::string &mnemonic)
  {
    auto pos = mnemonic.find('M');
    return pos != std::string::npos;
  }

} // namespace hackasm

