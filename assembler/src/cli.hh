#ifndef _hackasm_cli_hpp_
#define _hackasm_cli_hpp_

#include <string>
#include <array>
#include <getopt.h>

namespace hackasm
{

/**
 * Defines the CLI (command line interface) for the assembler.
 */
class cli
{
public:
  enum hack_out_fmt 
  {
    /* Output assembled machine instructions in a binary file. */
    HACKOUT_BINARY,
  
    /* Output assembeled machine instructions in a text file. */
    HACKOUT_TEXT,
  };

  /** Parsed cli data. */
  struct settings
  {
    /** name of hack asm file to assemble. */
    std::string in_filename;

    /** name of hack machine instruction output file. */
    std::string out_filename;

    /** format of output hack machine instruction file. */
    hack_out_fmt out_fmt;

    /** help flag was passed? */
    bool help_bit;
  };

  /** Definition of all CLI options. */
  static constexpr const char* short_opts {"f:ho:"};
  static constexpr struct option long_opts[] = {
    /*-------------------------------------------------------
     * name      has_arg            n/a       short_name
     *-----------------------------------------------------*/
    { "format",  required_argument, nullptr,  'f' },
    { "help"  ,  no_argument      , nullptr,  'h' },
    { "out"   ,  required_argument, nullptr,  'o' },
  };

  /** Arguments to format cli option, e.g '-f bin' or '--format bin'. */
  static constexpr std::array<const char*, 2> out_fmt_strs = {
    "bin", "txt"
  };

  /** Parses the command line options into a struct for easy use. */
  const settings& parse(int argc, char* const* argv);
  
  /** Prints the programs help message to stdout. */
  static void print_help();

  const settings& get_settings() const {return _settings;}

private:
  static hack_out_fmt id_out_fmt(const char* out_fmt_str);

private:
  settings _settings;
};

} // namespace hackasm

#endif
