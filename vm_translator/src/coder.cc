#include <unordered_map>
#include <cassert>
#include "coder.hh"
#include "error.hh"

namespace hackvmt
{

  namespace
  {
    /**
     * These routines are internal routines which implement the vm
     * runtime environment and a few of the larger vm commands. They
     * are injected at the top of all output .asm files. Alternatively
     * they could be inlined for every use but this would bloat the
     * output. Instead each vm 'return' cmd, for example, will jump to
     * the 'return_routine'; the asm code injected at the return site
     * to perform the jump is much shorter than the return routine
     * itself.
     *
     * note: these routines expect the return ROM address to be assigned
     * to the D register prior to jumping to them. Thus all routines
     * start with the code: @R15, M=D, which extracts the return address
     * and stores it in R15 for later use.
     */
    constexpr const char* boot_routine {
      ""
    };
    constexpr const char* add_routine {
      "(ADD_START)\n"
      "@R15\n"
      "M=D\n"
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"
      "A=A-1\n"
      "M=D+M\n"
      "@R15\n"
      "A=M\n"
      "0;JMP\n"
    };
    constexpr const char* sub_routine {
      "(SUB_START)\n"
      "@R15\n"
      "M=D\n"
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"
      "A=A-1\n"
      "M=M-D\n"
      "@R15\n"
      "A=M\n"
      "0;JMP\n"
    };
    constexpr const char* eq_routine {
      "(EQ_START)\n"
      "@R15\n"
      "M=D\n"
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"
      "A=A-1\n"
      "D=M-D\n"
      "M=0\n"
      "@EQ_END\n"
      "D;JNE\n"
      "@SP\n"
      "A=M-1\n"
      "M=-1\n"
      "(EQ_END)\n"
      "@R15\n"
      "A=M\n"
      "0;JMP\n"
    };
    constexpr const char* gt_routine {
      "(GT_START)\n"
      "@R15\n"
      "M=D\n"
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"
      "A=A-1\n"
      "D=M-D\n"
      "M=0\n"
      "@GT_END\n"
      "D;JLE\n"
      "@SP\n"
      "A=M-1\n"
      "M=-1\n"
      "(GT_END)\n"
      "@R15\n"
      "A=M\n"
      "0;JMP\n"
    };
    constexpr const char* lt_routine {
      "(LT_START)\n"
      "@R15\n"
      "M=D\n"
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"
      "A=A-1\n"
      "D=M-D\n"
      "M=0\n"
      "@LT_END\n"
      "D;JGE\n"
      "@SP\n"
      "A=M-1\n"
      "M=-1\n"
      "(LT_END)\n"
      "@R15\n"
      "A=M\n"
      "0;JMP\n"
    };
    constexpr const char* and_routine {
      "(AND_START)\n"
      "@R15\n"
      "M=D\n"
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"
      "A=A-1\n"
      "M=D&M\n"
      "@R15\n"
      "A=M\n"
      "0;JMP\n"
    };
    constexpr const char* or_routine {
      "(OR_START)\n"
      "@R15\n"
      "M=D\n"
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"
      "A=A-1\n"
      "M=D|M\n"
      "@R15\n"
      "A=M\n"
      "0;JMP\n"
    };
    constexpr const char* call_routine {
      "(CALL_START)\n"
      "@SP\n"
      "A=M\n"
      "M=D\n"
      "@LCL\n"
      "D=M\n"
      "@SP\n"
      "AM=M+1\n"
      "M=D\n"
      "@ARG\n"
      "D=M\n"
      "@SP\n"
      "AM=M+1\n"
      "M=D\n"
      "@THIS\n"
      "D=M\n"
      "@SP\n"
      "AM=M+1\n"
      "M=D\n"
      "@THAT\n"
      "D=M\n"
      "@SP\n"
      "AM=M+1\n"
      "M=D\n"
      "@SP\n"
      "D=M\n"
      "@4\n"
      "D=D-A\n"
      "@R13\n"
      "D=D-M\n"
      "@ARG\n"
      "M=D\n"
      "@SP\n"
      "DM=M+1\n"
      "@LCL\n"
      "M=D\n"
      "@R14\n"
      "A=M\n"
      "0;JMP\n"
    };
    constexpr const char* return_routine {
      "(RETURN_START)\n"
      "@LCL\n"
      "D=M\n"
      "@R13\n"
      "M=D\n"
      "@5\n"
      "AD=D-A\n"
      "D=M\n"
      "@R14\n"
      "M=D\n"
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"
      "@ARG\n"
      "M=D\n"
      "D=A+1\n"
      "@SP\n"
      "M=D\n"
      "@R13\n"
      "AM=M-1\n"
      "D=M\n"
      "@THAT\n"
      "M=D\n"
      "@R13\n"
      "AM=M-1\n"
      "D=M\n"
      "@THIS\n"
      "M=D\n"
      "@R13\n"
      "AM=M-1\n"
      "D=M\n"
      "@ARG\n"
      "M=D\n"
      "@R13\n"
      "AM=M-1\n"
      "D=M\n"
      "@LCL\n"
      "M=D\n"
      "@R14\n"
      "A=M\n"
      "0;JMP\n"
    };

    /** Map between vm segment names and segment asm pointers. */
    const std::unordered_map<std::string, std::string> segmentMap {
      {"argument", "ARG"},
      {"local", "LCL"},
      {"this", "THIS"},
      {"that", "THAT"}
    };

    /** Map between pointer segment index and asm pointers. */
    const std::unordered_map<std::string, std::string> pointerSegmentMap {
      {"0", "THIS"},
      {"1", "THAT"},
    };

    /** The base RAM address of the temp memory segment. */
    auto temp_segment_ram_base = 5;
  }

  code_writer::code_writer(const std::string &out_filename)
  {
    _ofile.open(out_filename, std::ios::trunc);
    if(!_ofile){
      throw "";
    }
  }

  void code_writer::write_head()
  {
    _ofile << boot_routine;
    _ofile << add_routine;
    _ofile << sub_routine;
    _ofile << eq_routine;
    _ofile << gt_routine;
    _ofile << lt_routine;
    _ofile << and_routine;
    _ofile << or_routine;
    _ofile << call_routine;
    _ofile << return_routine;
  }

  void code_writer::write_code(const vm_cmd &cmd)
  {
    static std::string asm_code{};

    asm_code.clear();
    switch(cmd._type){
      case vm_cmd_type::CMD_PUSH:
        code_push(cmd, asm_code);
        break;
      case vm_cmd_type::CMD_POP:
        code_pop(cmd, asm_code);
        break;
      case vm_cmd_type::CMD_ARITHMETIC:
        code_arithmetic(cmd, asm_code);
        break;
      case vm_cmd_type::CMD_COMPARISON:
        code_comparison(cmd, asm_code);
        break;
      case vm_cmd_type::CMD_LOGICAL:
        code_logical(cmd, asm_code);
        break;
      case vm_cmd_type::CMD_CALL:
        code_call(cmd, asm_code);
        break;
      case vm_cmd_type::CMD_FUNCTION:
        code_function(cmd, asm_code);
        break;
      case vm_cmd_type::CMD_RETURN:
        code_return(cmd, asm_code);
        break;
      case vm_cmd_type::CMD_GOTO:
        code_goto(cmd, asm_code);
        break;
      case vm_cmd_type::CMD_IF_GOTO:
        code_if_goto(cmd, asm_code);
        break;
      case vm_cmd_type::CMD_LABEL:
        code_label(cmd, asm_code);
        break;
    }

    _ofile << asm_code;
    _ofile.flush();
  }

  void code_writer::set_class_name(const std::string &name)
  {
    _class_name = name;
  }

  void code_writer::set_function_name(const std::string &name)
  {
    _function_name = name;
  }

  void code_writer::code_push(const vm_cmd& cmd, std::string& out_code)
  {
    auto& segment = cmd._arg0;
    auto& index = cmd._arg1;

    if(segment == "constant"){
      out_code += '@';
      out_code += index;
      out_code += '\n';
      out_code += "D=A\n"
                  "@SP\n"
                  "A=M\n"
                  "M=D\n"
                  "@SP\n"
                  "M=M+1\n";
    }

    else if(segment == "pointer"){
      auto segment_ptr = pointerSegmentMap.find(index);
      assert(segment_ptr != pointerSegmentMap.end()); // parser should ensure impossibility.
      out_code += '@';
      out_code += segment_ptr->second;
      out_code += '\n';
      out_code += "D=M\n"
                  "@SP\n"
                  "A=M\n"
                  "M=D\n"
                  "@SP\n"
                  "M=M+1\n";
    }

    else if(segment == "temp"){
      auto index_as_int = std::stoi(index);
      index_as_int += temp_segment_ram_base;
      out_code += '@';
      out_code += std::to_string(index_as_int);
      out_code += '\n';
      out_code += "D=M\n"
                  "@SP\n"
                  "A=M\n"
                  "M=D\n"
                  "@SP\n"
                  "M=M+1\n";
    }

    // for statics we insert a new label that when later read by the assembler will
    // result in a new variable being created, starting from RAM 16.
    else if(segment == "static"){
      out_code += '@';
      out_code += _class_name;
      out_code += '.';
      out_code += index;
      out_code += '\n';
      out_code += "D=M\n"
                  "@SP\n"
                  "A=M\n"
                  "M=D\n"
                  "@SP\n"
                  "M=M+1\n";
    }

    // segment == local | argument | this | that
    else {
      auto segment_ptr = segmentMap.find(segment);
      assert(segment_ptr != pointerSegmentMap.end()); // parser should ensure impossibility.
      out_code += '@';
      out_code += index;
      out_code += '\n';
      out_code += "D=A\n";
      out_code += "@";
      out_code += segment_ptr->second;
      out_code += '\n';
      out_code += "A=D+M\n"
                  "D=M\n"
                  "@SP\n"
                  "A=M\n"
                  "M=D\n"
                  "@SP\n"
                  "M=M+1\n";
    }
  }

  void code_writer::code_pop(const vm_cmd& cmd, std::string& out_code)
  {
    auto& segment = cmd._arg0;
    auto& index = cmd._arg1;

    assert(segment != "argument");
    assert(segment != "constant");

    if(segment == "pointer"){
      auto segment_ptr = pointerSegmentMap.find(index);
      assert(segment_ptr != pointerSegmentMap.end()); // parser should ensure impossibility.
      out_code += "@SP\n"
                  "AM=M-1\n"
                  "D=M\n";
      out_code += '@';
      out_code += segment_ptr->second;
      out_code += '\n';
      out_code += "M=D\n";
    }

    else if(segment == "temp"){
      auto index_as_int = std::stoi(index);
      index_as_int += temp_segment_ram_base;
      out_code += "@SP\n"
                  "AM=M-1\n"
                  "D=M\n";
      out_code += '@';
      out_code += std::to_string(index_as_int);
      out_code += '\n';
      out_code += "M=D\n";
    }

    else if(segment == "static"){
      out_code += "@SP\n"
                  "AM=M-1\n"
                  "D=M\n";
      out_code += '@';
      out_code += _class_name;
      out_code += '.';
      out_code += index;
      out_code += '\n';
      out_code += "M=D\n";
    }

    // segment == local | this | that
    else {
      auto segment_ptr = segmentMap.find(segment);
      assert(segment_ptr != pointerSegmentMap.end()); // parser should ensure impossibility.
      out_code += '@';
      out_code += index;
      out_code += '\n';
      out_code += "D=A\n";
      out_code += "@";
      out_code += segment_ptr->second;
      out_code += '\n';
      out_code += "D=D+M\n"
                  "@R15\n"
                  "M=D\n"
                  "@SP\n"
                  "AM=M-1\n"
                  "D=M\n"
                  "@R15\n"
                  "A=M\n"
                  "M=D\n";
    }
  }

  void code_writer::code_arithmetic(const vm_cmd& cmd, std::string& out_code)
  {
    static auto add_call_count = 0;
    static auto sub_call_count = 0;

    auto return_label = std::string{};
    auto jump_label = std::string{};

    if(cmd._keyword == "add"){
      return_label = "RET_ADDRESS_ADD";
      return_label += std::to_string(add_call_count++);
      jump_label = "ADD_START";
    }

    else if(cmd._keyword == "sub"){
      return_label = "RET_ADDRESS_SUB";
      return_label += std::to_string(sub_call_count++);
      jump_label = "SUB_START";
    }

    // note: inlining this cmd since it is short.
    else if(cmd._keyword == "neg"){
      out_code += "@SP\n"
                  "A=M-1\n"
                  "M=-M\n";
      return;
    }

    else assert(0);

    out_code += '@';
    out_code += return_label;
    out_code += '\n';
    out_code += "D=A\n";
    out_code += '@';
    out_code += jump_label;
    out_code += '\n';
    out_code += "0;JMP\n";
    out_code += '(';
    out_code += return_label;
    out_code += ")\n";
  }

  void code_writer::code_comparison(const vm_cmd& cmd, std::string& out_code)
  {
    static auto eq_call_count = 0;
    static auto gt_call_count = 0;
    static auto lt_call_count = 0;

    auto return_label = std::string{};
    auto jump_label = std::string{};

    if(cmd._keyword == "eq"){
      return_label = "RETURN_ADDRESS_EQ";
      return_label += std::to_string(eq_call_count++);
      jump_label = "EQ_START";
    }

    else if(cmd._keyword == "gt"){
      return_label = "RETURN_ADDRESS_GT";
      return_label += std::to_string(gt_call_count++);
      jump_label = "GT_START";
    }

    else if(cmd._keyword == "lt"){
      return_label = "RETURN_ADDRESS_LT";
      return_label += std::to_string(lt_call_count++);
      jump_label = "LT_START";
    }

    else assert(0);

    out_code += '@';
    out_code += return_label;
    out_code += '\n';
    out_code += "D=A\n";
    out_code += '@';
    out_code += jump_label;
    out_code += '\n';
    out_code += "0;JMP\n";
    out_code += '(';
    out_code += return_label;
    out_code += ")\n";
  }

  void code_writer::code_logical(const vm_cmd& cmd, std::string& out_code)
  {
    static auto and_call_count = 0;
    static auto or_call_count = 0;

    auto return_label = std::string{};
    auto jump_label = std::string{};

    if(cmd._keyword == "and"){
      return_label = "RETURN_ADDRESS_AND";
      return_label += std::to_string(and_call_count++);
      jump_label = "AND_START";
    }

    else if(cmd._keyword == "or"){
      return_label = "RETURN_ADDRESS_OR";
      return_label += std::to_string(or_call_count++);
      jump_label = "OR_START";
    }

    // note: inlining this cmd since it is short.
    else if(cmd._keyword == "not"){
      out_code += "@SP\n"
                  "A=M-1\n"
                  "M=!M\n";
      return;
    }

    else assert(0);

    out_code += '@';
    out_code += return_label;
    out_code += '\n';
    out_code += "D=A\n";
    out_code += '@';
    out_code += jump_label;
    out_code += '\n';
    out_code += "0;JMP\n";
    out_code += '(';
    out_code += return_label;
    out_code += ")\n";
  }

  void code_writer::code_call(const vm_cmd& cmd, std::string& out_code)
  {
    const auto& function_name = cmd._arg0;
    const auto& arg_count = cmd._arg1;

    auto jump_label = _class_name;
    jump_label += '.';
    jump_label += function_name;

    auto return_label = _class_name;
    return_label += '.';
    return_label += function_name;
    return_label += "$ret.";
    return_label += std::to_string(_call_count++);

    out_code += '@';
    out_code += arg_count;
    out_code += '\n';
    out_code += "D=A\n"
                "@R13\n"
                "M=D\n";
    out_code += '@';
    out_code += jump_label;
    out_code += '\n';
    out_code += "D=A\n"
                "@R14\n"
                "M=D\n";
    out_code += '@';
    out_code += return_label;
    out_code += '\n';
    out_code += "D=A\n";
    out_code += "@CALL_START\n"
                "0;JMP\n";
    out_code += '(';
    out_code += return_label;
    out_code += ")\n";
  }

  void code_writer::code_function(const vm_cmd& cmd, std::string& out_code)
  {
    const auto& function_name = cmd._arg0;
    const auto& var_count = cmd._arg1;

    auto entry_label = _class_name;
    entry_label += '.';
    entry_label += function_name;

    out_code += '(';
    out_code += entry_label;
    out_code += ")\n";

    auto var_count_as_int = std::stoi(var_count);
    while(var_count_as_int > 0){
      out_code += "@SP\n"
                  "AM=M+1\n"
                  "A=A-1\n"
                  "M=0\n";
      --var_count_as_int;
    }
  }

  void code_writer::code_return(const vm_cmd& cmd, std::string& out_code)
  {
    out_code = "@RETURN_START\n"
               "0;JMP\n";
  }

  void code_writer::code_goto(const vm_cmd& cmd, std::string& out_code)
  {
    auto label = cmd._arg0;

    auto goto_label = _function_name;
    goto_label += '$';
    goto_label += label;

    out_code += '@';
    out_code += goto_label;
    out_code += '\n';
    out_code += "0;JMP\n";
  }

  void code_writer::code_if_goto(const vm_cmd& cmd, std::string& out_code)
  {
    auto label = cmd._arg0;

    auto goto_label = _function_name;
    goto_label += '$';
    goto_label += label;

    out_code += "@SP\n"
                "AM=M-1\n"
                "D=M\n";
    out_code += '@';
    out_code += goto_label;
    out_code += '\n';
    out_code += "D;JNE\n";
  }

  void code_writer::code_label(const vm_cmd& cmd, std::string& out_code)
  {
    auto label = cmd._arg0;

    auto goto_label = _function_name;
    goto_label += '$';
    goto_label += label;

    out_code += '(';
    out_code += goto_label;
    out_code += ")\n";
  }

} // namespace hackvmt
