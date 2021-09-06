#ifndef _HACKASM_CODER_HH_
#define _HACKASM_CODER_HH_

#include <unordered_map>
#include <optional>
#include <cstdint>
#include "cmd.hh"

namespace hackasm
{

/**
 * The Hack computer has 32Kib of RAM and ROM, thus
 *  addresses lie within the range [0, 32767].
 */
using hack_address = uint16_t;

class coder 
{
public:
  coder();

  /** Adds a label to the symbol table. Throws if the label is already added. */
  void add_label(const std::string& symbol, hack_address address);

  /**
   * Translates an asm_cmd to a bin_cmd. Throws if cannot translate. Does not
   * accept L_CMDs as theses do not produce hack machine instructions.
   */
  bin_cmd code(const asm_cmd& cmd);

  static constexpr hack_address initial_ram_address = 23;

private:
  /** Maps between assembly mnemonics and binary instruction codes. */
  static const std::unordered_map<std::string, uint16_t> comp_codes;
  static const std::unordered_map<std::string, uint16_t> dest_codes;
  static const std::unordered_map<std::string, uint16_t> jump_codes;

  /** Defines the basic set of symbols defined by the hack specification. */
  static const std::unordered_map<std::string, hack_address> base_symbols;

  bin_cmd code_a_cmd(const asm_cmd& cmd);
  bin_cmd code_c0_cmd(const asm_cmd& cmd);
  bin_cmd code_c1_cmd(const asm_cmd& cmd);
  bin_cmd code_c2_cmd(const asm_cmd& cmd);

  static uint16_t get_dest_code(const std::string& mnemonic);
  static uint16_t get_comp_code(const std::string& mnemonic);
  static uint16_t get_jump_code(const std::string& mnemonic);

  static bool references_m(const std::string& mnemonic);

  std::unordered_map<std::string, hack_address> _symbol_table;
  hack_address _next_ram_adress;
};

} // namespace hackasm

#endif
