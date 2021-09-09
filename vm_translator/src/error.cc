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

  unknown_cli_option::unknown_cli_option(int opt) noexcept
  {
    std::stringstream ss{};
    ss << "unknown option '" << logbold << std::to_string(opt) << logstd << "'\n";
    _msg = ss.str();
  }

  cli_missing_required_arg::cli_missing_required_arg(char option) noexcept
  {
    std::stringstream ss{};
    ss << "option '" << logbold << option << logstd << "' missing argument";
    _msg = std::string{ss.str()};
  }

  cli_missing_input::cli_missing_input() noexcept
  {
    _msg = "missing input files; nothing to translate.";
  }

  cli_missing_main::cli_missing_main() noexcept
  {
    _msg = "missing Main.vm input file; program has no entry point.";
  }

  cli_expected_vm_file::cli_expected_vm_file(const std::string &received) noexcept
  {
    std::stringstream ss{};
    ss << "expected .vm file, received '" << logbold << received << logstd << "'.";
    _msg = std::string{ss.str()};
  }

} // namespace hackvmt
