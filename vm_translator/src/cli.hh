#ifndef NAND2TETRIS_CLI_HH
#define NAND2TETRIS_CLI_HH

#include <vector>
#include <string>
#include <getopt.h>

namespace hackvmt
{

  /** Defines the command line interface for the vm translator. */
  class cli
  {
  public:
    /** Parsed cli data. */
    struct settings
    {
      /** name of hack vm file/s to translate. */
      std::vector<std::string> in_filenames;

      /** name of hack output assembly file. */
      std::string out_filename = "a.asm";

      /** help flag was passed? */
      bool help_bit = false;
    };

    /** Definition of all CLI options. */
    static constexpr const char* short_opts {":f:ho:"};
    static constexpr struct option long_opts[] = {
      /*-------------------------------------------------------
       * name      has_arg            n/a       short_name
       *-----------------------------------------------------*/
      { "help"  ,  no_argument      , nullptr,  'h' },
      { "out"   ,  required_argument, nullptr,  'o' },
    };

    /** Parses the command line options into a struct for easy use. */
    const settings& parse(int argc, char* const* argv);

    /** Prints the programs help message to stdout. */
    static void print_help();

    [[nodiscard]] const settings& get_settings() const {return _settings;}

  private:
    static bool is_vm_filename(const std::string& candidate);
    settings _settings;
  };

}

#endif //NAND2TETRIS_CLI_HH
