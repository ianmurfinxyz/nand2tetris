#include "log.hh"

namespace hackvmt
{

  void log(log_level level, const char* message)
  {
    switch(level) {
      case log_level::FATAL:
        std::cout << logfatal << "fatal error: ";
        break;
      case log_level::ERROR:
        std::cout << logerror << "error: ";
        break;
      case log_level::WARN:
        std::cout << logwarn << "warning: ";
        break;
      case log_level::INFO:
        std::cout << loginfo << "info: ";
        break;
    }
    std::cout << logstd << message << std::endl;
  }

} // namespace hackasm
