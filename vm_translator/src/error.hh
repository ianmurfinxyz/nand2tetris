#ifndef NAND2TETRIS_ERROR_HH
#define NAND2TETRIS_ERROR_HH

#include <exception>
#include <string>

namespace hackvmt
{

  class vmt_error : public std::exception
  {
  public:
    explicit vmt_error() noexcept = default;
    explicit vmt_error(std::string msg) noexcept;
    virtual ~vmt_error() noexcept = default;
    vmt_error(const vmt_error&) noexcept = default;
    vmt_error& operator=(const vmt_error&) noexcept = default;
    [[nodiscard]] virtual const char* what() const noexcept;
  protected:
    std::string _msg;
  };

  class vmfile_open_error : public vmt_error
  {
  public:
    explicit vmfile_open_error(const std::string& filename);
  };

  class expected_token : public vmt_error
  {
  public:
    explicit expected_token(const std::string& expected, const std::string& received);
  };

  class unknown_cli_option: public vmt_error
  {
  public:
    explicit unknown_cli_option(int opt) noexcept;
  };

  class cli_missing_required_arg : public vmt_error
  {
  public:
    explicit cli_missing_required_arg(char option) noexcept;
  };

  class cli_missing_input : public vmt_error
  {
  public:
    explicit cli_missing_input() noexcept;
  };

  class cli_missing_main : public vmt_error
  {
  public:
    explicit cli_missing_main() noexcept;
  };

  class cli_expected_vm_file : public vmt_error
  {
  public:
    explicit cli_expected_vm_file(const std::string& received) noexcept;
  };

} // namespace hackvmt

#endif //NAND2TETRIS_ERROR_HH
