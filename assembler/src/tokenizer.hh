#ifndef _HACKASM_TOKENISER_HPP_
#define _HACKASM_TOKENISER_HPP_

#include <sstream>
#include <string>
#include <string_view>
#include <fstream>
#include <array>
#include <optional>
#include <iostream>

namespace hackasm
{
  /**
   * note: unfortunately there is some overlap between the token
   * sets thus meaning the set a token belongs to cannot be determined
   * by looking at the token alone. This sucks! :( It means my parser
   * has to be complicated and rather ugly!
   *
   * conclusion: a compiler like architecture for the hack assembly
   * language doesn't appear to work very well. Perhaps a simple regex
   * approach would of been better.
   */
  enum class token_type
  {
    dest,
    comp,
    comp_or_int_const,
    comp_or_dest,
    jump,
    identifier,
    symbol,
    int_const,
  };

  std::ostream& operator<<(std::ostream&, token_type tt);

  struct token
  {
    token_type _type;
    std::string _value;
  };

  constexpr std::array<const char*, 28> comp_or_int_const_tokens {
    "0"  ,
    "1"  ,
  };
  constexpr std::array<const char*, 25> comp_tokens {
    "-1" ,
    "!D" ,
    "!A" ,
    "-D" ,
    "-A" ,
    "D+1",
    "A+1",
    "D-1",
    "A-1",
    "D+A",
    "D-A",
    "A-D",
    "D&A",
    "D|A",
    "!M" ,
    "-M" ,
    "M+1",
    "M-1",
    "D+M",
    "D-M",
    "M-D",
    "D&M",
    "D|M",
  };
  constexpr std::array<const char*, 4> dest_tokens {
    "DM" ,
    "AM" ,
    "AD" ,
    "ADM",
  };
  constexpr std::array<const char*, 3> comp_or_dest_tokens {
    "M"  ,
    "D"  ,
    "A"  ,
  };
  constexpr std::array<const char*, 7> jump_tokens {
    "JGT",
    "JEQ",
    "JGE",
    "JLT",
    "JNE",
    "JLE",
    "JMP",
  };
  constexpr std::array<char, 7> symbol_tokens {
    '@',
    '=',
    ';',
    '(',
    ')',
  };

  static constexpr const char comment_prefix = '#';

  class tokenizer
  {
  public:
    explicit tokenizer(const std::string& asm_filename);
    std::optional<token> advance();
  private:
    char peek_skip_ws_and_comments();
    static bool is_symbol(char c);
    static bool is_int_const(const token& t);
    token_type id_token(const token& t) const;

    std::ifstream _asm_file;
    token _curr_token;
  };

} // namespace hackasm

#endif