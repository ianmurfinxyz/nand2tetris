#include <array>
#include <regex>
#include "parser.hh"
#include "util.hh"
#include "error.hh"

using namespace std::string_literals;

namespace hackasm
{
  namespace
  {
    constexpr auto max_line_length = 100;
    auto comment_rx_pattern  {R"(^\s*#+([\s\S]*)$)"s};
    auto empty_ws_rx_pattern {R"(^\s*$)"s};
    auto l_cmd_rx_pattern    {R"(^\s*\({1}\s*(\w+)\s*\){1}\s*$)"s};
    auto a_cmd_rx_pattern    {R"(^\s*@{1}\s*(\w+)\s*$)"s};
    auto c0_cmd_rx_pattern   {R"(^\s*(\w+)={1}([\s\S]+);{1}(\w+)$)"s};
    auto c1_cmd_rx_pattern   {R"(^\s*(\w+)\s*={1}([\s\S]+)$)"s};
    auto c2_cmd_rx_pattern   {R"(^([\s\S]+);{1}([\s\S]+)$)"s};
  }

  parser::parser(const std::string& asm_filename) : _lineno{0}
  {
    _asm_file.open(asm_filename);
    if(!_asm_file) {
      throw asm_file_not_found{asm_filename};
    }
  }

  std::optional<asm_cmd> parser::advance()
  {
    static std::array<char, max_line_length> buff;

    static const std::regex comment_rx  {comment_rx_pattern};
    static const std::regex empty_ws_rx {empty_ws_rx_pattern};
    static const std::regex l_cmd_rx    {l_cmd_rx_pattern};
    static const std::regex a_cmd_rx    {a_cmd_rx_pattern};
    static const std::regex c0_cmd_rx   {c0_cmd_rx_pattern};
    static const std::regex c1_cmd_rx   {c1_cmd_rx_pattern};
    static const std::regex c2_cmd_rx   {c2_cmd_rx_pattern};

    auto match = std::cmatch{};
    auto rcount = 0L;
    auto skip = false;
    auto cmd = asm_cmd{};

    do {
      _asm_file.getline(buff.data(), buff.size());
      ++_lineno;
      rcount = _asm_file.gcount();
      skip = std::regex_match(buff.data(), match, comment_rx  ) ||
             std::regex_match(buff.data(), match, empty_ws_rx ) ||
             rcount == 1;
    }
    while(skip && _asm_file);

    // run out of data and last line was junk.
    if(skip && !_asm_file){
      return {};
    }

    if(std::regex_match(buff.data(), match, l_cmd_rx)){
      cmd._type = cmd_type::L_CMD;
      cmd._symbol = strip_ws(match[1]);
    }
    else if(std::regex_match(buff.data(), match, a_cmd_rx)){
      cmd._type = cmd_type::A_CMD;
      cmd._symbol = match[1];
      if(is_int_const(cmd._symbol)){
        cmd._address = std::move(cmd._symbol);
      }
    }
    else if(std::regex_match(buff.data(), match, c0_cmd_rx)){
      cmd._type = cmd_type::C0_CMD;
      cmd._dest = strip_ws(match[1]);
      cmd._comp = strip_ws(match[2]);
      cmd._jump = strip_ws(match[3]);
    }
    else if(std::regex_match(buff.data(), match, c1_cmd_rx)){
      cmd._type = cmd_type::C1_CMD;
      cmd._dest = strip_ws(match[1]);
      cmd._comp = strip_ws(match[2]);
    }
    else if(std::regex_match(buff.data(), match, c2_cmd_rx)){
      cmd._type = cmd_type::C2_CMD;
      cmd._comp = strip_ws(match[1]);
      cmd._jump = strip_ws(match[2]);
    }
    else {
      throw invalid_asm_cmd{std::string{buff.data()}, _lineno};
    }

    return cmd;
  }

} // hackasm
