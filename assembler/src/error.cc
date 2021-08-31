#include <sstream>
#include "error.hh"
#include "log.hh"

namespace hackasm
{
  asm_error::asm_error(std::string msg) noexcept : _msg{std::move(msg)}{}

  const char* asm_error::what() const noexcept
  {
    return _msg.c_str();
  }

  cli_asm_file_arg_missing::cli_asm_file_arg_missing() noexcept
  {
    _msg = std::string{"no asm file input"};
  }

  excess_cli_args::excess_cli_args() noexcept
  {
    _msg = std::string{"too many arguments"};
  }

  cli_missing_required_arg::cli_missing_required_arg(char option) noexcept
  {
    std::stringstream ss{};
    ss << "option '" << logbold << option << logstd << "' missing argument";
    _msg = std::string{ss.str()};
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

  unknown_out_format::unknown_out_format(const std::string& out_fmt) noexcept
  {
    std::stringstream ss{};
    ss << "unrecognised output format argument '" << logbold << out_fmt << logstd << "'\n" ;
    _msg = ss.str();
  }

  invalid_asm_cmd::invalid_asm_cmd(const std::string& asm_cmd_str, int lineno) noexcept
  {
    std::stringstream ss{};
    ss << "invalid assembly instruction on line [" << logbold << lineno << logstd << "]\n"
       << "\t\t'" << logbold << asm_cmd_str << logstd << "'\n" ;
    _msg = ss.str();
  }

  duplicate_label::duplicate_label(const std::string& label) noexcept
  {
    std::stringstream ss{};
    ss << "duplicate label '" << logbold << label << logstd << "'\n";
    _msg = std::string{ss.str()};
  }

  hack_address_out_of_range::hack_address_out_of_range(long long address) noexcept
  {
    std::stringstream ss{};
    ss << "RAM address '" << logbold << address << logstd << "' out of range\n";
    _msg = std::string{ss.str()};
  }

  unknown_mnemonic::unknown_mnemonic(const std::string& type, const std::string& mnemonic) noexcept
  {
    std::stringstream ss{};
    ss << "unknown " << type << " mnemonic '" << logbold << mnemonic << logstd << "'\n";
    _msg = std::string{ss.str()};
  }

  a_register_conflict::a_register_conflict(const std::string &asm_cmd_str) noexcept
  {
    std::stringstream ss{};
    ss << "conflicting use of A register in instruction '" << logbold << asm_cmd_str << logstd << "'\n";
    _msg = std::string{ss.str()};
  }
} // namespace hackasm
