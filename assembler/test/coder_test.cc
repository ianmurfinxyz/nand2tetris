#include <gtest/gtest.h>
#include "cmd.hh"
#include "error.hh"
#include "coder.hh"

hackasm::asm_cmd build_a_symbol_cmd(const char* symbol)
{
  return hackasm::asm_cmd {
    hackasm::cmd_type::A_CMD,
    std::string{symbol},
    "",
    "",
    "",
    ""
  };
}

hackasm::asm_cmd build_a_address_cmd(const char* address)
{
  return hackasm::asm_cmd {
    hackasm::cmd_type::A_CMD,
    "",
    std::string{address},
    "",
    "",
    ""
  };
}

hackasm::asm_cmd build_c0_cmd(const char* dest, const char* comp, const char* jump)
{
  return hackasm::asm_cmd {
    hackasm::cmd_type::C0_CMD,
    "",
    "",
    std::string{dest},
    std::string{comp},
    std::string{jump}
  };
}

hackasm::asm_cmd build_c1_cmd(const char* dest, const char* comp)
{
  return hackasm::asm_cmd {
    hackasm::cmd_type::C0_CMD,
    "",
    "",
    std::string{dest},
    std::string{comp},
    ""
  };
}

hackasm::asm_cmd build_c2_cmd(const char* comp, const char* jump)
{
  return hackasm::asm_cmd {
    hackasm::cmd_type::C0_CMD,
    "",
    "",
    "",
    std::string{comp},
    std::string{jump}
  };
}

TEST(coder_test, code_cmds) {
  auto coder = hackasm::coder{};
  auto asm_cmd = hackasm::asm_cmd{};
  auto bin_cmd = hackasm::bin_cmd{};

  EXPECT_NO_THROW(coder.add_label("LABEL1", 0x01));
  EXPECT_NO_THROW(coder.add_label("LABEL2", 0x02));
  EXPECT_NO_THROW(coder.add_label("LABEL3", 0x03));

  EXPECT_THROW(coder.add_label("LABEL1", 0x01), hackasm::duplicate_label);
  EXPECT_THROW(coder.add_label("LABEL2", 0x02), hackasm::duplicate_label);
  EXPECT_THROW(coder.add_label("LABEL3", 0x03), hackasm::duplicate_label);

  asm_cmd = build_a_address_cmd("20");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 20);
}
