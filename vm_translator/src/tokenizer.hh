#ifndef NAND2TETRIS_TOKENIZER_HH
#define NAND2TETRIS_TOKENIZER_HH

#include <string>
#include <optional>
#include <fstream>
#include "vm_cmd.hh"

namespace hackvmt
{

  class tokenizer
  {
  public:
    explicit tokenizer(const std::string& vm_filename);
    std::optional<token_type> advance();
  private:
    static bool is_space(int c);
    bool is_comment(int c0);
    void skip_comment();
    std::ifstream _vmfile;
    token_type _token;
  };

} // namespace hackvmt

#endif //NAND2TETRIS_TOKENIZER_HH
