#ifndef _HACKASM_WRITER_HH_
#define _HACKASM_WRITER_HH_

#include <fstream>
#include <string>
#include "cmd.hh"

namespace hackasm
{
  class writer
  {
  public:
    explicit writer(const std::string& filename, std::ios_base::openmode mode);
    virtual ~writer() = default;
    virtual void write(bin_cmd cmd) = 0;
  protected:
    std::ofstream _ofile;
  };

  class txt_writer final: public writer
  {
  public:
    explicit txt_writer(const std::string& filename);
    void write(bin_cmd cmd) override;
  };
}

#endif
