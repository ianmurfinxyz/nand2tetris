#ifndef NAND2TETRIS_PARSER_HH
#define NAND2TETRIS_PARSER_HH

#include <string>
#include <optional>
#include <unordered_map>
#include "tokenizer.hh"

namespace hackvmt
{
  /**
   * Parses the hack VM grammar which consists of the
   * following rules:
   *
   *  push command: 'push' segment index
   *
   *  pop command: 'pop' segment index
   *
   *  arithmetic commands: 'add'|'sub'|'neg'
   *
   *  comparison commands: 'eq'|'gt'|'lt'
   *
   *  logical commands: 'and'|'or'|'not'
   *
   *  call command: 'call' f nArgs
   *
   *  function cmd: 'function' f nVars
   *
   *  return command: 'return'
   *
   *  goto command: 'goto' label
   *
   *  if-goto command: 'if-goto' label
   *
   *  label command: 'label' label
   *
   *  where segment is:
   *    'argument'|'local'|'static'|'constant'|'this'|'that'|'pointer'|'temp'
   */
  class parser
  {
  public:
    using token_type = tokenizer::token_type;

    enum class vm_cmd_type
    {
      CMD_PUSH,
      CMD_POP,
      CMD_ARITHMETIC,
      CMD_COMPARISON,
      CMD_LOGICAL,
      CMD_CALL,
      CMD_FUNCTION,
      CMD_RETURN,
      CMD_GOTO,
      CMD_IF_GOTO,
      CMD_LABEL,
    };

    struct vm_cmd
    {
      vm_cmd_type _type;
      token_type _keyword;
      token_type _arg0;
      token_type _arg1;
    };

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
