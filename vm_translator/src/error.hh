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
    vmfile_open_error(const std::string& filename);
  };

} // namespace hackvmt

#endif //NAND2TETRIS_ERROR_HH
