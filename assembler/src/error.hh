#ifndef _HACKASM_ERROR_HH_
#define _HACKASM_ERROR_HH_

#include <string>
#include <exception>
#include "coder.hh"

namespace hackasm
{

/** Base class for assembly exceptions. */
class asm_error : public std::exception
{
public:
  explicit asm_error() noexcept = default;
  explicit asm_error(const std::string& msg) noexcept;
  virtual ~asm_error() noexcept = default;
  asm_error(const asm_error&) noexcept = default;
  asm_error& operator=(const asm_error&) noexcept = default;
  [[nodiscard]] virtual const char* what() const noexcept;
protected:
  std::string _msg;
};

class unknown_cli_option: public asm_error
{
public:
  explicit unknown_cli_option(int opt) noexcept;
};

class unknown_out_format : public asm_error
{
public:
  explicit unknown_out_format(const std::string& out_fmt) noexcept;
};

class cli_asm_file_arg_missing : public asm_error
{
public:
  explicit cli_asm_file_arg_missing() noexcept;
};

class excess_cli_args : public asm_error
{
public:
  explicit excess_cli_args() noexcept;
};

class cli_missing_required_arg : public asm_error
{
public:
  explicit cli_missing_required_arg(char option) noexcept;
};


class asm_file_not_found: public asm_error
{
public:
  explicit asm_file_not_found(const std::string& filename) noexcept;
};

class invalid_asm_cmd: public asm_error
{
public:
  explicit invalid_asm_cmd(const std::string& asm_cmd_str, int lineno) noexcept;
};

class multiple_label_declarations: public asm_error
{
public:
  explicit multiple_label_declarations(
      const std::string& label, 
      hack_address prev, 
      hack_address curr
  ) noexcept;
};
 
} // namespace hackasm

#endif
