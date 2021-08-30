#include <iostream>
#include <cstring>
#include "cli.hh"
#include "error.hh"
#include "log.hh"

namespace hackasm
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
        case 'f':
          _settings.out_fmt = id_out_fmt(optarg);
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
    if(diff == 1) {
      _settings.in_filename = std::string{argv[optind]};
    }
    else if(diff > 1){
      throw excess_cli_args{};
    }
    else {
      throw cli_asm_file_arg_missing{};
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
              << "  --format=txt         outputs ascii hack executable.\n\n"
              << "txt format is for use with the official Nand2Tetris emulator,\n"
              << "whereas binary format is used for the emulator packaged with \n"
              << "this suite of tools.\n\n"
              << "source availible at: https://github.com/ianmurfinxyz/nand2tetris \n"
              << std::endl;
  }

  cli::hack_out_fmt cli::id_out_fmt(const char* out_fmt_str)
  {
    int index = hack_out_fmt::HACKOUT_BINARY;
    for(auto fmt : out_fmt_strs) {
      if(std::strcmp(fmt, out_fmt_str) == 0){
        return static_cast<hack_out_fmt>(index);
      }
      ++index;
    }
    throw unknown_out_format{out_fmt_str};
  }

} // namespace hackasm
