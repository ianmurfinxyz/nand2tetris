#include <cstdlib>
#include <iostream>
#include "cli.hh"
#include "error.hh"
#include "tokenizer.hh"

int main(int argc, char* argv[])
{
  using namespace hackasm;
  auto cli_ = cli{};
  try{
    cli_.parse(argc, argv);
  }
  catch(const asm_error& e){
    std::cout << e.what() << std::endl;
    cli::print_help();
  }
  const auto& settings = cli_.get_settings();
  if(settings.help_bit) {
    cli::print_help();
    std::exit(EXIT_SUCCESS);
  }

  auto tokenizer_ = tokenizer{settings.in_filename};
  auto token = tokenizer_.advance();
  while(token.has_value()) {
    std::cout << token.value()._type << "\n";
    std::cout << token.value()._value << "\n";
    std::cout << std::endl;
  }


  // get filename from args
  // load file
  //
  // phase 1:
  //  read line-by-line looking for symbols, adding them to table
  //
  // phase 2:
  //  parse line-by-line
  //  code asm instruction into machine instruction
  //  write out to new hack program file.
}