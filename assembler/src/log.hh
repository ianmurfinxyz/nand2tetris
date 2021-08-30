#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <iostream>
#include <array>
#include <string>
#include "error.hpp"

namespace hackasm
{

/** Escape sequences to control logging colors; useful for
 * highlighting output. */
const char* logfatal {"\e[1;31m"};
const char* logerror {"\e[1;31m"};
const char* logwarn {"\e[1;33m"};
const char* loginfo {"\e[1;36m"};
const char* loghl {"\e[1;32m"};
const char* logbold {"\e[1m"};
const char* logstd {"\e[0m"};


enum class LogLevel { FATAL, ERROR, WARN, INFO };

void log(LogLevel level, const char* message);

} // namespace hackasm

#endif
