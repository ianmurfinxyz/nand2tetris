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

enum class token_type
{
  dest,
  comp,
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

constexpr std::array<const char*, 28> comp_tokens {
  "0"  ,  
  "1"  ,  
  "-1" ,  
  "D"  ,  
  "A"  ,  
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
  "M"  ,  
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
constexpr std::array<const char*, 7> dest_tokens {
  "M"  ,  
  "D"  ,  
  "DM" ,  
  "A"  ,  
  "AM" ,  
  "AD" ,  
  "ADM",  
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

class tokenizer
{
public:
  explicit tokenizer(const std::string& asm_filename);
  std::optional<token> advance();
private:
  char peek_skip_ws();
  static bool is_symbol(char c);
  static bool is_int_const(const token& t);
  token_type id_token(const token& t) const;

  std::ifstream _asm_file;
  token _curr_token;
};

} // namespace hackasm

#endif