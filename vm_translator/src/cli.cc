#include <iostream>
#include <regex>
#include "cli.hh"
#include "error.hh"
#include "log.hh"

namespace hackvmt
{
  const cli::settings& cli::parse(int argc, char* const* argv)
  {
    optind = 1; /* resets the getopt lib so can unit test */
    auto opt_index {0};
    auto opt {0};
    while((opt = getopt_long(argc, argv, short_opts, long_opts, &opt_index)) != -1){
      switch(opt){
        case 'o':
          _settings.out_filename = std::string{optarg};
          break;
        case 'h':
          _settings.help_bit = true;
          break;
        case '?':
          throw unknown_cli_option{opt};
        default:
          throw cli_missing_required_arg{static_cast<char>(opt)};
      }
    }
    if(_settings.help_bit){
      return _settings;
    }
    const auto diff = argc - optind;
    if(diff == 0){
      throw cli_missing_input{};
    }
    for(auto i {optind}; i < argc; ++i){
      auto in_filename = std::string{argv[optind]};
      if(!is_vm_filename(in_filename)){
        throw cli_expected_vm_file{in_filename};
      }
      _settings.in_filenames.push_back(std::move(in_filename));
    }
    return _settings;
  }

  void cli::print_help()
  {
    std::cout << logbold << "USAGE\n" << logstd
              << "  hackasm [OPTION]... FILE\n\n"
              << "assemble hack assembly file to hack executable file.\n\n"
              << logbold << "OPTIONS\n" << logstd
              << "  -o, --out=FILE       output filename\n"
              << "  -f, --format=FORMAT  output file format\n"
              << "  -h, --help           display this help text\n\n"
              << logbold << "OUTPUT FORMATS\n" << logstd
              << "  --format=bin         outputs binary hack executable.\n"
              << "  --format=txt         outputs ascii hack executable (default).\n\n"
              << "txt format is for use with the official Nand2Tetris emulator,\n"
              << "whereas binary format is used for the emulator packaged with \n"
              << "this suite of tools.\n\n"
              << "source availible at: https://github.com/ianmurfinxyz/nand2tetris \n"
              << std::endl;
  }

  bool cli::is_vm_filename(const std::string &candidate)
  {
    static auto pattern {R"(^[\w_.\s]+$)"};
    static std::regex rx {pattern, std::regex::ECMAScript};
    auto match = std::smatch{};
    return std::regex_match(candidate, match, rx) && match[0].length() == candidate.length();
  }

} // namespace hackasm
