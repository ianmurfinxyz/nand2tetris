#include <sstream>
#include "error.hh"
#include "log.hh"

namespace hackasm
{

asm_error::asm_error(const std::string& msg) noexcept : _msg{msg}{}

const char* asm_error::what() const noexcept
{
  return _msg.c_str();
}

asm_file_not_found::asm_file_not_found(const std::string& filename) noexcept
{
  std::stringstream ss{};
  ss << "hack assembly file " << logbold << "'" << filename << "'" << logstd
     << "does not exist!\n";
  _msg = std::string{ss.str()};
}

unknown_cli_option::unknown_cli_option(int opt) noexcept
{
  std::stringstream ss{};
  ss << "unknown option '" << logbold << std::to_string(opt) << logstd << "'\n";
  _msg = ss.str();
}

unknown_out_format::unknown_out_format(const std::string &out_fmt) noexcept
{
  std::stringstream ss{};
  ss << "unrecognised output format argument '" << logbold << out_fmt << logstd << "'\n" ;
  _msg = ss.str();
}

multiple_label_declarations::multiple_label_declarations(
  const std::string& label, 
  hack_address prev, 
  hack_address curr
) noexcept :
  asm_error{}
{
  std::stringstream ss{}; 
  ss << "multiple declarations of label '"
     << loghl << label << logstd
     << "'; prior declaration had address '"
     << std::hex << logbold << prev << logstd
     << "', new declaration has address '" 
     << std::hex << logbold << curr << logstd
     << "'\n";
  _msg = std::string{ss.str()};
}

} // namespace hackasm
