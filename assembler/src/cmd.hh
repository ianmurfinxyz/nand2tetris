#ifndef _HACKASM_CMD_HH_
#define _HACKASM_CMD_HH_

#include <cstdint>
#include <string>

namespace hackasm
{
  enum class cmd_type
  {
    L_CMD,  // (<symbol>)
    A_CMD,  // @(<symbol>|<int_const>)
    C0_CMD, // <dest>=<comp>;<jump>
    C1_CMD, // <dest>=<comp>
    C2_CMD, // <comp>;<jump>
  };

  /** Represents a hack assembly command. */
  struct asm_cmd
  {
    cmd_type _type;
    std::string _symbol;
    std::string _address;
    std::string _dest;
    std::string _comp;
    std::string _jump;
  };

  /** Represents a hack binary command. */
  using bin_cmd = uint16_t;

} // namespace hackasm

#endif
