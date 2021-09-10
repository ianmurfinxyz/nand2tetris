#include <array>
#include <cassert>
#include <algorithm>
#include <regex>
#include "parser.hh"
#include "error.hh"

namespace hackvmt
{
  std::unordered_map<std::string, vm_cmd_type> parser::keyword_cmd_map {
    {"push", vm_cmd_type::CMD_PUSH},
    {"pop", vm_cmd_type::CMD_POP},
    {"add", vm_cmd_type::CMD_ARITHMETIC},
    {"sub", vm_cmd_type::CMD_ARITHMETIC},
    {"neg", vm_cmd_type::CMD_ARITHMETIC},
    {"eq", vm_cmd_type::CMD_COMPARISON},
    {"gt", vm_cmd_type::CMD_COMPARISON},
    {"lt", vm_cmd_type::CMD_COMPARISON},
    {"and", vm_cmd_type::CMD_LOGICAL},
    {"or", vm_cmd_type::CMD_LOGICAL},
    {"not", vm_cmd_type::CMD_LOGICAL},
    {"call", vm_cmd_type::CMD_CALL},
    {"function", vm_cmd_type::CMD_FUNCTION},
    {"return", vm_cmd_type::CMD_RETURN},
    {"goto", vm_cmd_type::CMD_GOTO},
    {"if-goto", vm_cmd_type::CMD_IF_GOTO},
    {"label", vm_cmd_type::CMD_LABEL},
  };

  namespace
  {
    /** The memory segments that can be pushed/popped to/from. */
    const std::array<std::string, 10> segments {
      "argument",
      "local",
      "static",
      "constant",
      "this",
      "that",
      "pointer",
      "temp"
    };
  }

  parser::parser(const std::string &vm_filename) : _tokenizer{vm_filename} {}

  std::optional<vm_cmd> parser::advance()
  {
    auto keyword = extract_keyword();
    if(!keyword.has_value()){
      return {}; // no more tokens; eof.
    }
    _cmd._type = get_cmd_type(keyword.value());
    _cmd._keyword = keyword.value();
    switch(_cmd._type){
      case vm_cmd_type::CMD_PUSH:
      case vm_cmd_type::CMD_POP: {
        auto segment = extract_segment();
        auto index = extract_number("index");
        if(segment == "pointer"){
          if(!(index == "0" || index == "1")){
            throw invalid_pointer_segment_index{index};
          }
        }
        else if(segment == "temp"){
          auto index_as_int = std::stoi(index);
          if(!(0 <= index_as_int && index_as_int <= 7)){
            throw invalid_temp_segment_index{index};
          }
        }
        _cmd._arg0 = std::move(segment);
        _cmd._arg1 = std::move(index);
        break;
      }
      case vm_cmd_type::CMD_CALL:
        _cmd._arg0 = extract_label();
        _cmd._arg1 = extract_number("nArgs");
        break;
      case vm_cmd_type::CMD_FUNCTION:
        _cmd._arg0 = extract_label();
        _cmd._arg1 = extract_number("nVars");
        break;
      case vm_cmd_type::CMD_GOTO:
      case vm_cmd_type::CMD_IF_GOTO:
      case vm_cmd_type::CMD_LABEL:
        _cmd._arg0 = extract_label();
        break;
      case vm_cmd_type::CMD_ARITHMETIC:
      case vm_cmd_type::CMD_COMPARISON:
      case vm_cmd_type::CMD_LOGICAL:
      case vm_cmd_type::CMD_RETURN:
        break;
    }
    return _cmd;
  }

  vm_cmd_type parser::get_cmd_type(const token_type& keyword)
  {
    auto iter = keyword_cmd_map.find(keyword);
    if(iter == keyword_cmd_map.end()){
      assert(0);
    }
    return iter->second;
  }

  bool parser::is_keyword(const token_type &candidate)
  {
    return keyword_cmd_map.contains(candidate);
  }

  bool parser::is_segment(const token_type& candidate)
  {
    return std::find(segments.begin(), segments.end(), candidate) != segments.end();
  }

  bool parser::is_number(const token_type& candidate)
  {
    return std::ranges::all_of(candidate.begin(), candidate.end(), [](auto c){return std::isdigit(c);});
  }

  bool parser::is_label(const token_type &candidate)
  {
    static auto pattern {R"(^[[a-zA-Z_:.]{1}[\w_:.]*$)"};
    static std::regex rx {pattern, std::regex::ECMAScript};

    auto match = std::smatch{};

    // note: std::regex::multiline is not supported yet, so instead compare the length to
    // ensure we match the full candidate.
    return std::regex_match(candidate, match, rx) && match[0].length() == candidate.length();
  }

  std::optional<token_type> parser::extract_keyword()
  {
    auto token = _tokenizer.advance();
    if(!token.has_value()){
      return {};
    }
    if(!is_keyword(token.value())){
      throw expected_token{"keyword", token.value()};
    }
    return token.value();
  }

  token_type parser::extract_segment()
  {
    auto token = _tokenizer.advance();
    if(!token.has_value()){
      throw expected_token{"segment", "end of file"};
    }
    if(!is_segment(token.value())){
      throw expected_token{"segment", token.value()};
    }
    return token.value();
  }

  token_type parser::extract_number(const char* number_context)
  {
    auto token = _tokenizer.advance();
    if(!token.has_value()){
      throw expected_token{number_context, "end of file"};
    }
    if(!is_number(token.value())){
      throw expected_token{number_context, token.value()};
    }
    return token.value();
  }

  token_type parser::extract_label()
  {
    auto token = _tokenizer.advance();
    if(!token.has_value()){
      throw expected_token{"label", "end of file"};
    }
    if(!is_label(token.value())){
      throw expected_token{"label", token.value()};
    }
    return token.value();
  }

} // namespace hackvmt