#ifndef _HACKASM_WRITER_HH_
#define _HACKASM_WRITER_HH_

#include "cmd.hh"

namespace hackasm
{
  class writer
  {
  public:
    void write(bin_cmd cmd);
  };
}

#endif
