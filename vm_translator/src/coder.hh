#ifndef NAND2TETRIS_CODER_HH
#define NAND2TETRIS_CODER_HH

#include <fstream>
#include <string>
#include <vector>
#include "vm_cmd.hh"

namespace hackvmt
{

  class code_writer
  {
  public:
    code_writer(const std::string& out_filename);

    /** Writes the out file head code. */
    void write_head();

    /** Translates a vm_cmd to hack assembly and writes to out file. */
    void write_code(const vm_cmd& cmd);

    void set_class_name(const std::string& name);
    void set_function_name(const std::string& name);

  private:
    void code_push(const vm_cmd& cmd, std::string& out_code);
    void code_pop(const vm_cmd& cmd, std::string& out_code);
    void code_arithmetic(const vm_cmd& cmd, std::string& out_code);
    void code_comparison(const vm_cmd& cmd, std::string& out_code);
    void code_logical(const vm_cmd& cmd, std::string& out_code);
    void code_call(const vm_cmd& cmd, std::string& out_code);
    void code_function(const vm_cmd& cmd, std::string& out_code);
    void code_return(const vm_cmd& cmd, std::string& out_code);
    void code_goto(const vm_cmd& cmd, std::string& out_code);
    void code_if_goto(const vm_cmd& cmd, std::string& out_code);
    void code_label(const vm_cmd& cmd, std::string& out_code);

    std::ofstream _ofile;
    std::string _class_name;
    std::string _function_name;
  };

} // namespace hackvmt

#endif //NAND2TETRIS_CODER_HH
