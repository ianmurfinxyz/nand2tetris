#include "writer.hh"
#include "error.hh"
#include "cmd.hh"

namespace hackasm
{
  writer::writer(const std::string& filename, std::ios_base::openmode mode)
  {
    auto mode_ = mode | std::ios_base::out;
    _ofile.open(filename, mode_);
    if(!_ofile){
      throw ofile_open_error{filename};
    }
  }

  txt_writer::txt_writer(const std::string &filename) :
    writer(filename, std::ios_base::trunc)
  {}

  void txt_writer::write(bin_cmd cmd)
  {
    constexpr auto bit_count = sizeof(bin_cmd) * 8;
    constexpr char bit_chars[2] = {'0', '1'};
    static char bitset[bit_count + 1];
    bitset[bit_count] = '\0';
    for(auto bit_index{0}; bit_index < bit_count; ++bit_index){
      auto bit_val = (cmd >> bit_index) & 0x1;
      bitset[(bit_count - 1) - bit_index] = bit_chars[bit_val];
    }
    _ofile << bitset << std::endl;
    _ofile.flush();
  }

} // namespace hackasm