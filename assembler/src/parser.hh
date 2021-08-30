#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <fstream>
#include <string>
#include <optional>
#include "cmd.hh"

namespace hackasm
{
  class parser
  {
  public:
    explicit parser(const std::string& asm_filename);
    std::optional<asm_cmd> advance();
  private:
    std::ifstream _asm_file;
    asm_cmd _asm_cmd;
    int _lineno;
  };

} // namespace hackasm

#endif
