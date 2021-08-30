#include <algorithm>
#include <cctype>
#include <cstring>
#include "tokenizer.hh"
#include "error.hh"

namespace hackasm
{
  std::ostream& operator<<(std::ostream&, token_type tt)
  {
    switch(tt){
      case token_type::dest:
        std::cout << "dest";
      case token_type::comp:
        std::cout << "comp";
      case token_type::comp_or_int_const:
        std::cout << "comp_or_int_const";
      case token_type::comp_or_dest:
        std::cout << "comp_or_dest";
      case token_type::jump:
        std::cout << "jump";
      case token_type::identifier:
        std::cout << "identifier";
      case token_type::symbol:
        std::cout << "symbol";
      case token_type::int_const:
        std::cout << "int_const";
    }
    return std::cout;
  }

  tokenizer::tokenizer(const std::string& asm_filename)
  {
    _asm_file.open(asm_filename);
    if(!_asm_file) {
      throw asm_file_not_found{asm_filename};
    }
  }

  std::optional<token> tokenizer::advance()
  {
    auto peek_c = peek_skip_ws_and_comments();
    if(peek_c == _asm_file.eof()){
      return {};
    }
    _curr_token._value.clear();
    if(is_symbol(peek_c)){
      _curr_token._type = token_type::symbol;
      _curr_token._value.push_back(_asm_file.get());
      return _curr_token;
    }
    else {
      while(peek_c != _asm_file.eof() && !std::iscntrl(peek_c) && !is_symbol(peek_c)){
        auto c = _asm_file.get();
        if(!std::isspace(peek_c)){
          _curr_token._value.push_back(c);
        }
        peek_c = _asm_file.peek();
      }
      _curr_token._type = id_token(_curr_token);
      return _curr_token;
    }
  }

  char tokenizer::peek_skip_ws_and_comments()
  {
    auto eof = _asm_file.eof();
    auto c = _asm_file.peek();
    while(c != eof && (std::isspace(c) || c == comment_prefix)){
      if(c == comment_prefix){
        while(c != eof && !std::iscntrl(c)) {
          _asm_file.get();
          c = _asm_file.peek();
        }
      }
      else {
        _asm_file.get();
        c = _asm_file.peek();
      }
    }
    return c;
  }

  bool tokenizer::is_symbol(char c)
  {
    auto symbol_equal = [c](auto symbol_token){return c == symbol_token;};
    auto result = std::find_if(symbol_tokens.begin(), symbol_tokens.end(), symbol_equal);
    return result != symbol_tokens.end();
  }

  bool tokenizer::is_int_const(const token &t)
  {
    return std::ranges::all_of(t._value.begin(), t._value.end(), [](auto c){return std::isdigit(c);});
  }

  token_type tokenizer::id_token(const token& t) const
  {
    auto token_equal = [t](auto& other){return std::strcmp(t._value.c_str(), other) == 0;};

    auto result = std::find_if(comp_or_int_const_tokens.begin(), comp_or_int_const_tokens.end(), token_equal);
    if(result != comp_or_int_const_tokens.end()){
      return token_type::comp_or_int_const;
    }

    result = std::find_if(comp_or_dest_tokens.begin(), comp_or_dest_tokens.end(), token_equal);
    if(result != comp_or_dest_tokens.end()){
      return token_type::comp_or_dest;
    }

    result = std::find_if(comp_tokens.begin(), comp_tokens.end(), token_equal);
    if(result != comp_tokens.end()){
      return token_type::comp;
    }

    result = std::find_if(dest_tokens.begin(), dest_tokens.end(), token_equal);
    if(result != dest_tokens.end()){
      return token_type::dest;
    }


    result = std::find_if(jump_tokens.begin(), jump_tokens.end(), token_equal);
    if(result != jump_tokens.end()){
      return token_type::jump;
    }

    if(is_int_const(t)){
      return token_type::int_const;
    }

    // any unknown string is assumed to be an identifier.
    return token_type::identifier;
  }
} // namespace hackasm
