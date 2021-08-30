#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <fstream>
#include "cmd.hpp"

namespace hackasm
{

class parser
{
public:
  /** Opens a hack assembly file, ready for parsing.
   * throws:
   *  file_not_found - if fail to load file */
  parser(const std::string& filename);

  /**
   * Advances to the next instruction in the file, returning the parsed result;
   * all whitespace and comments are skipped.
   */
  bool advance(asm_cmd& cmd);

private:
  std::ifstream _asmfile;
};

} // namespace hackasm

#endif
