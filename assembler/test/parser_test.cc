#include <gtest/gtest.h>
#include "parser.hh"
#include "error.hh"
#include "cmd.hh"

TEST(parser_test, parse_sum_asm) {
  auto parser = hackasm::parser{"../assembler/test/sum.asm"};
  auto cmd = std::optional<hackasm::asm_cmd>{};

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._address.c_str(), "10");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "D");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "A");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "R0");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "M");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "D");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "i");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "M");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "1");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "sum");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "M");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "0");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::L_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "LOOP");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "i");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "D");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "M");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "R0");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "D");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "D-M");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "STOP");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C2_CMD);
  EXPECT_STREQ(cmd.value()._comp.c_str(), "D");
  EXPECT_STREQ(cmd.value()._jump.c_str(), "JGT");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "sum");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "D");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "M");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "i");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "D");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "D+M");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "sum");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "M");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "D");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "i");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "M");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "M+1");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "LOOP");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C2_CMD);
  EXPECT_STREQ(cmd.value()._comp.c_str(), "0");
  EXPECT_STREQ(cmd.value()._jump.c_str(), "JMP");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::L_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "STOP");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "sum");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "D");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "M");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "R1");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C1_CMD);
  EXPECT_STREQ(cmd.value()._dest.c_str(), "M");
  EXPECT_STREQ(cmd.value()._comp.c_str(), "D");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::L_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "END");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::A_CMD);
  EXPECT_STREQ(cmd.value()._symbol.c_str(), "END");

  cmd = std::move(parser.advance());
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd.value()._type, hackasm::cmd_type::C2_CMD);
  EXPECT_STREQ(cmd.value()._comp.c_str(), "0");
  EXPECT_STREQ(cmd.value()._jump.c_str(), "JMP");

  EXPECT_THROW(parser.advance(), hackasm::invalid_asm_cmd);
}

TEST(parser_test, parse_missing_asm_file) {
  EXPECT_THROW(hackasm::parser{"missing_file"}, hackasm::asm_file_not_found);
}
