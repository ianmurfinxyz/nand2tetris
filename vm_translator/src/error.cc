#include <sstream>
#include "error.hh"
#include "log.hh"

namespace hackvmt
{

  vmt_error::vmt_error(std::string msg) noexcept : _msg{std::move(msg)}{}

  const char* vmt_error::what() const noexcept
  {
    return _msg.c_str();
  }

  vmfile_open_error::vmfile_open_error(const std::string &filename)
  {
    std::stringstream ss{};
    ss << "failed to open input file '" << logbold << filename << logstd << "'.";
    _msg = ss.str();
  }

  expected_token::expected_token(const std::string &expected, const std::string& received)
  {
    std::stringstream ss{};
    ss << "expected " << logbold << expected << logstd
       << "' token but received " << logbold << received << logstd << ".";
    _msg = ss.str();
  }

} // namespace hackvmt
