#ifndef NAND2TETRIS_VM_CMD_HH
#define NAND2TETRIS_VM_CMD_HH

#include <string>

namespace hackvmt
{
  /**
   * The hack VM grammar consists of the following rules:
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

  using token_type = std::string;

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

} // namespace hackvmt

#endif //NAND2TETRIS_VM_CMD_HH
