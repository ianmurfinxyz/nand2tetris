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

/**
 * note: There are too many possible commands to test every single one, thus
 * this test takes the approach of choosing a random but broad subset of
 * commands and testing only those. The assumption is that if all those tested
 * are without error then all those untested will likely be too, provided the
 * test cases are sufficiently broad.
 *
 * for c0 cmds: (form <dest>=<comp>;<jump>)
 *  valid dest mnemonics: (count 3)
 *      D, A, AD
 *  valid comp mnemonics: (count 18)
 *      0, 1, -1, D, A, !D, !A, -D, -A, D+1, A+1, D-1, A-1, D+A, D-A, A-D, D&A, D|A
 *  valid jump mnemonics: (count 7)
 *      JGT, JEQ, JGE, JLT, JNE, JLE, JMP
 *  giving a total of 3*18*7=378 possible c0 cmds.
 *
 * for c1 cmds: (form <dest>=<comp>)
 *  valid dest mnemonics: (count 7)
 *      M, D, DM, A, AM, AD, ADM
 *  valid comp mnemonics: (count 28)
 *      0, 1, -1, D, A, !D, !A, -D, -A, D+1, A+1, D-1, A-1, D+A, D-A, A-D, D&A, D|A,
 *      M, !M, -M, M+1, M-1, D+M, M-D, D&M, D|M
 *  giving a total of 7*28=196 possible c1 cmds.
 *
 * for c2 cmds: (form <comp>;<jump>)
 *  valid comp mnemonics: (count 18)
 *      0, 1, -1, D, A, !D, !A, -D, -A, D+1, A+1, D-1, A-1, D+A, D-A, A-D, D&A, D|A
 *  valid jump mnemonics: (count 7)
 *      JGT, JEQ, JGE, JLT, JNE, JLE, JMP
 *  giving a total of 7*18=126 possible c1 cmds.
 *
 * thus total c commands: 378+196+126=700
 */

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
