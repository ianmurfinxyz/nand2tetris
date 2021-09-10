#ifndef NAND2TETRIS_PARSER_HH
#define NAND2TETRIS_PARSER_HH

#include <string>
#include <optional>
#include <unordered_map>
#include "tokenizer.hh"
#include "vm_cmd.hh"

namespace hackvmt
{
  class parser
  {
  public:
    explicit parser(const std::string& vm_filename);
    std::optional<vm_cmd> advance();

  private:
    /** Maps command keywords to their command type. */
    static std::unordered_map<std::string, vm_cmd_type> keyword_cmd_map;

    static bool is_keyword(const token_type& candidate);
    static bool is_segment(const token_type& candidate);
    static bool is_number(const token_type& candidate);
    static bool is_label(const token_type& candidate);

    static vm_cmd_type get_cmd_type(const token_type& keyword);

    std::optional<token_type> extract_keyword();
    token_type extract_segment();
    token_type extract_number(const char* number_context);
    token_type extract_label();

    tokenizer _tokenizer;
    vm_cmd _cmd;
  };

} // namespace hackvmt

#endif //NAND2TETRIS_PARSER_HH
