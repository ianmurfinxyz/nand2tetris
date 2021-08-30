#ifndef _hackasm_cmd_hpp_
#define _hackasm_cmd_hpp_

#include <cstdint>
#include <string>

enum class cmd_type
{
  L_CMD, 
  A_CMD, 
  C_CMD,
};

/** Represents an assembly command. */
struct asm_cmd 
{
  cmd_type _type;
  std::string _symbol;
  std::string _address;
  std::string _dest;
  std::string _comp;
  std::string _jump;
};

using bin_cmd = uint16_t;

#endif
