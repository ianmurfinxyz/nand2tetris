#include <cstdlib>
#include <iostream>
#include "cli.hh"
#include "error.hh"
#include "parser.hh"
#include "writer.hh"

int main(int argc, char** argv)
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

  auto coder_ = coder{};
  auto parser_ = parser{settings.in_filename};

  // phase 1: add all labels to the symbol table.
  auto asm_cmd_ = parser_.advance();
  while(asm_cmd_.has_value()){
    if(asm_cmd_.value()._type == cmd_type::L_CMD){
      coder.add_symbol(asm_cmd_.value()._symbol);
    }
    asm_cmd_ = std::move(parser_.advance());
  }

  // phase 2: translate all asm_cmds to bin_cmds.
  parser_.reset();
  auto writer_ = writer{};
  asm_cmd_ = parser_.advance();
  while(asm_cmd_.has_value()){
    auto bin_cmd_ = coder_.code(asm_cmd_.value());
    writer._write(bin_cmd_);
    asm_cmd_ = std::move(parser_.advance());
  }
}