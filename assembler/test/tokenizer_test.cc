#include <gtest/gtest.h>
#include "tokenizer.hh"

TEST(tokenizer_test, tokenize_sum) {
  hackasm::tokenizer tokenizer{"../assembler/test/sum.asm"};
  std::optional<hackasm::token> token{};

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::symbol);
  EXPECT_STREQ(token.value()._value.c_str(), "@");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::identifier);
  EXPECT_STREQ(token.value()._value.c_str(), "R0");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::comp_or_dest);
  EXPECT_STREQ(token.value()._value.c_str(), "M");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::symbol);
  EXPECT_STREQ(token.value()._value.c_str(), "=");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::int_const);
  EXPECT_STREQ(token.value()._value.c_str(), "10");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::symbol);
  EXPECT_STREQ(token.value()._value.c_str(), "@");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::identifier);
  EXPECT_STREQ(token.value()._value.c_str(), "i");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::comp_or_dest);
  EXPECT_STREQ(token.value()._value.c_str(), "M");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::symbol);
  EXPECT_STREQ(token.value()._value.c_str(), "=");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::comp_or_int_const);
  EXPECT_STREQ(token.value()._value.c_str(), "1");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::symbol);
  EXPECT_STREQ(token.value()._value.c_str(), "@");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::identifier);
  EXPECT_STREQ(token.value()._value.c_str(), "sum");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::comp_or_dest);
  EXPECT_STREQ(token.value()._value.c_str(), "M");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::symbol);
  EXPECT_STREQ(token.value()._value.c_str(), "=");

  token = tokenizer.advance();
  ASSERT_TRUE(token.has_value());
  EXPECT_EQ(token.value()._type, hackasm::token_type::comp_or_int_const);
  EXPECT_STREQ(token.value()._value.c_str(), "0");
}
