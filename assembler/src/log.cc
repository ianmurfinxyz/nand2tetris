#include "log.hpp"
#include "error.hpp"

namespace hackasm 
{

static const std::array<const char*, LogLevel::INFO + 1> levelString = {{
  "FATAL", "ERROR", "WARN", "INFO"
}};

void log(LogLevel level, const char* message)
{
  // todo: make some pretty colored output for the level string.
  std::cout << levelString[level] << message << std::endl;
}

} // namespace hackasm
