#include <fstream>
#include "parser.hpp"
#include "error.hpp"

namespace hackasm
{

parser::parser(const std::string& filename) : _asmfile{}
{
  _asmfile.open(filename); 
  if(!_asmfile.is_open()) {
    throw infile_not_found{filename};
  }
}

bool parser::advance(asm_cmd& cmd)
{
  
}

} // hackasl
