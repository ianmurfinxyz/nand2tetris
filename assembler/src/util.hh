#ifndef NAND2TETRIS_UTIL_HH
#define NAND2TETRIS_UTIL_HH

namespace hackasm
{
  /**
   * Tests if a string contains only digits and is therefore
   * a valid decimal constant.
   */
  bool is_int_const(const std::string& str);

  /**
   * Strips all whitespace from a string.
   */
  std::string strip_ws(std::string str);
}

#endif //NAND2TETRIS_UTIL_HH
