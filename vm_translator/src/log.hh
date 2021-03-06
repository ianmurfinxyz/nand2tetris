#ifndef NAND2TETRIS_LOGGER_HH
#define NAND2TETRIS_LOGGER_HH

#include <iostream>
#include <array>
#include <string>

namespace hackvmt
{

  /** Escape sequences to control logging colors; useful for
   * highlighting output. */
  constexpr const char* logfatal {"\e[1;31m"};
  constexpr const char* logerror {"\e[1;31m"};
  constexpr const char* logwarn {"\e[1;33m"};
  constexpr const char* loginfo {"\e[1;36m"};
  constexpr const char* logbold {"\e[1m"};
  constexpr const char* logstd {"\e[0m"};

  enum class log_level { FATAL, ERROR, WARN, INFO };

  void log(log_level level, const char* message);

} // namespace hackasm

#endif
