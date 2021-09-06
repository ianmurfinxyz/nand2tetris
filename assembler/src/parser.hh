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

    /** Read the next asm_cmd; skips ws and comments. Throws if parse error. */
    std::optional<asm_cmd> advance();

    /** Return the parse position to the file start. */
    void reset();

  private:
    std::ifstream _asm_file;
    int _lineno;
  };
} // namespace hackasm

#endif
