#include <cstdlib>
#include <iostream>
#include "cli.hh"
#include "error.hh"
#include "parser.hh"
#include "coder.hh"
#include "writer.hh"
#include "log.hh"

using namespace hackasm;

namespace
{
  auto error_bit = false;
}

std::optional<asm_cmd> parser_advance(parser& parser_)
{
  std::optional<asm_cmd> cmd{};
  auto success = true;
  do {
    try {
      cmd = parser_.advance();
      success = true;
    }
    catch(invalid_asm_cmd& e) {
      log(log_level::ERROR, e.what());
      success = false;
      error_bit = true;
    }
  }
  while(!success);
  return cmd;
}

void coder_add_label(coder& coder_, std::string& label, hack_address rom_address)
{
  try {
    coder_.add_label(label, rom_address);
  }
  catch(duplicate_label& e) {
    log(log_level::ERROR, e.what());
    error_bit = true;
  }
}

void populate_labels(parser& parser_, coder& coder_)
{
  hack_address rom_address = 0;
  auto asm_cmd_ = parser_advance(parser_);
  while(asm_cmd_.has_value()){
    if(asm_cmd_.value()._type == cmd_type::L_CMD){
      coder_add_label(coder_, asm_cmd_.value()._symbol, rom_address);
    }
    else {
      ++rom_address; // labels don't produce bin_cmds.
    }
    asm_cmd_ = std::move(parser_advance(parser_));
  }
}

bin_cmd coder_code(coder& coder_, const asm_cmd& cmd)
{
  bin_cmd bin_cmd_ = 0;
  try {
    bin_cmd_ = coder_.code(cmd);
  }
  catch(a_register_conflict& e){
    log(log_level::ERROR, e.what());
    log(log_level::INFO,
      "A C-instruction that specifies a reference to M should specify no jump, "
      "and visa versa; this is a best-practice to avoid conflicting use of the A register."
    );
    error_bit = true;
  }
  catch(asm_error& e){
    log(log_level::ERROR, e.what());
    error_bit = true;
  }
  return bin_cmd_;
}

void assemble(parser& parser_, coder& coder_, writer& writer_)
{
  auto asm_cmd_ = parser_.advance();
  while(asm_cmd_.has_value()){
    if(asm_cmd_.value()._type != cmd_type::L_CMD){
      auto bin_cmd_ = coder_code(coder_, asm_cmd_.value());
      if(!error_bit){
        writer_.write(bin_cmd_);
      }
    }
    asm_cmd_ = std::move(parser_.advance());
  }
}

void abort_check()
{
  if(error_bit){
    log(log_level::FATAL, "aborting assembly due to errors.");
    std::exit(EXIT_SUCCESS);
  }
}

int main(int argc, char** argv)
{
  auto cli_ = cli{};
  try {
    cli_.parse(argc, argv);
  }
  catch(const asm_error& e){
    std::cout << e.what() << std::endl;
    cli::print_help();
  }
  const auto& settings = cli_.get_settings();
  if(settings.help_bit){
    cli::print_help();
    std::exit(EXIT_SUCCESS);
  }

  auto coder_ = coder{};
  auto parser_ = parser{settings.in_filename};
  auto writer_ = writer{};

  populate_labels(parser_, coder_);
  abort_check();
  parser_.reset();
  assemble(parser_, coder_, writer_);
  abort_check();
}