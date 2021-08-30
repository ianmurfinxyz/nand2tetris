#ifndef _hackasm_coder_hpp_
#define _hackasm_coder_hpp_

#include <unordered_map>
#include <cstdint>
#include "cmd.hh"

namespace hackasm
{

/** The Hack computer has 32Kib of RAM and ROM, thus 
 *  addresses lie within the range [0, 32767]. */
using hack_address = uint32_t;

class coder 
{
public:
  coder();

  /** Declares an A symbol with the coder. If the symbol is already declared,
   * the coder does nothing, else the coder adds the symbol to the symbol 
   * table; mapping the symbol to the next free RAM address. */
  void declare_variable(const std::string& symbol);

  /** Declares an L symbol with the coder. If the symbol is already declared,
   * the coder throws, else the coder adds the new symbol to the symbol table. */
  void declare_label(const std::string& symbol, hack_address address);

  /** Codes a hack binary instruction from a hack assembly instruction. */
  bin_cmd code(const asm_cmd& cmd);

private:
  /** Maps between assembly mnemonics and binary instruction codes. */
  static const std::unordered_map<const char*, uint16_t> comp_codes;
  static const std::unordered_map<const char*, uint16_t> dest_codes;
  static const std::unordered_map<const char*, uint16_t> jump_codes;

  /** Defines pre-defined symbols common to all hack asm programs. */
  static const std::unordered_map<const char*, hack_address> common_symbol_table;

  /** Finds the address of 'symbol' if symbol is defined. If symbol is 
   *  not defined returns bad_address. */
  hack_address find_symbol_address(const std::string& symbol);

private:
  std::unordered_map<std::string, hack_address> _symbol_table;
  hack_address _next_ram_adress;
};

} // namespace hackasm

#endif
