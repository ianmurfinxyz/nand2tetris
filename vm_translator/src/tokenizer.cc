#include <cctype>
#include "tokenizer.hh"
#include "error.hh"

namespace hackvmt
{
  constexpr const char* comment_prefex {"//"};

  tokenizer::tokenizer(const std::string& vm_filename)
  {
    _vmfile.open(vm_filename);
    if(!_vmfile){
      throw vmfile_open_error{vm_filename};
    }
  }

  std::optional<tokenizer::token_type> tokenizer::advance()
  {
    _token.clear();

    // search for the 1st char of the next token.
    char c;
    while(_vmfile.get(c)){
      if(is_comment(c)){
        skip_comment();
      }
      else if(!is_space(c)){
        break;
      }
    }

    // if no more tokens.
    if(c == _vmfile.eof()){
      return {};
    }

    _token.push_back(c);

    // extract remainder of the token.
    while(_vmfile.get(c)){
      if(is_space(c)){
        break;
      }
      _token.push_back(c);
    }

    return _token;
  }

  bool tokenizer::is_comment(int c)
  {
    if(c != comment_prefex[0]){
      return false;
    }
    return _vmfile.peek() == comment_prefex[1];
  }

  void tokenizer::skip_comment()
  {
    char c;
    while(_vmfile.get(c)){
      if(c == '\n'){
        break;
      }
    }
  }

  bool tokenizer::is_space(int c)
  {
    return std::isspace(static_cast<unsigned char>(c));
  }

} // namespace hackvmt