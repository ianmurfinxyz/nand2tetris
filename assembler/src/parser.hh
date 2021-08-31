#ifndef _PARSER_HH_
#define _PARSER_HH_

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
    int _lineno;
  };

} // namespace hackasm

#endif
