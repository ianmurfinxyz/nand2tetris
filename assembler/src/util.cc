#include <string>
#include <cctype>
#include <algorithm>

namespace hackasm
{
  bool is_int_const(const std::string& str)
  {
    return std::ranges::all_of(str.begin(), str.end(), [](auto c){return std::isdigit(c);});
  }

  std::string strip_ws(std::string str)
  {
    auto start = std::remove_if(str.begin(), str.end(), [=](const char c){return std::isspace(c);});
    str.erase(start, str.end());
    return str;
  }

}

