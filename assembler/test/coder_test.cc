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
    hackasm::cmd_type::C1_CMD,
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
    hackasm::cmd_type::C2_CMD,
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
 *
 * To reduce the number of tests whilst still providing sufficient breadth I will write,
 * for each C_CMD type, tests that use all mnemonics at least once. Meaning the upper
 * limit of the required tests equals to the number of unique mnemonics rather than the
 * unique combinations.
 */

TEST(coder_test, code_cmds) {
  auto coder = hackasm::coder{};
  auto asm_cmd = hackasm::asm_cmd{};
  auto bin_cmd = hackasm::bin_cmd{};

  // A cmds

  EXPECT_NO_THROW(coder.add_label("LABEL1", 12345));
  EXPECT_NO_THROW(coder.add_label("LABEL2", 23456));
  EXPECT_NO_THROW(coder.add_label("LABEL3", 34567));

  EXPECT_THROW(coder.add_label("LABEL1", 0x01), hackasm::duplicate_label);
  EXPECT_THROW(coder.add_label("LABEL2", 0x02), hackasm::duplicate_label);
  EXPECT_THROW(coder.add_label("LABEL3", 0x03), hackasm::duplicate_label);

  asm_cmd = build_a_address_cmd("20");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 20);

  asm_cmd = build_a_symbol_cmd("LABEL1");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 12345);

  asm_cmd = build_a_symbol_cmd("LABEL2");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 23456);

  asm_cmd = build_a_symbol_cmd("LABEL3");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 34567);

  // C0 cmds

  asm_cmd = build_c0_cmd("D", "0", "JGT");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110101010010001);

  asm_cmd = build_c0_cmd("A", "1", "JEQ");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110111111100010);

  asm_cmd = build_c0_cmd("AD", "-1", "JGE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110111010110011);

  asm_cmd = build_c0_cmd("D", "D", "JLT");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001100010100);

  asm_cmd = build_c0_cmd("A", "A", "JNE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110000100101);

  asm_cmd = build_c0_cmd("AD", "!D", "JLE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001101110110);

  asm_cmd = build_c0_cmd("D", "!A", "JMP");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110001010111);

  asm_cmd = build_c0_cmd("A", "-D", "JGT");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001111100001);

  asm_cmd = build_c0_cmd("D", "-A", "JEQ");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110011010010);

  asm_cmd = build_c0_cmd("AD", "D+1", "JGE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110011111110011);

  asm_cmd = build_c0_cmd("A", "A+1", "JLT");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110111100100);

  asm_cmd = build_c0_cmd("D", "D-1", "JNE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001110010101);

  asm_cmd = build_c0_cmd("AD", "A-1", "JLE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110010110110);

  asm_cmd = build_c0_cmd("A", "D+A", "JMP");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110000010100111);

  asm_cmd = build_c0_cmd("A", "D-A", "JMP");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110010011100111);

  asm_cmd = build_c0_cmd("A", "A-D", "JMP");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110000111100111);

  asm_cmd = build_c0_cmd("A", "D&A", "JMP");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110000000100111);

  asm_cmd = build_c0_cmd("A", "D|A", "JMP");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110010101100111);

  // C1 cmds

  asm_cmd = build_c1_cmd("M", "0");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110101010001000);

  asm_cmd = build_c1_cmd("D", "1");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110111111010000);

  asm_cmd = build_c1_cmd("DM", "-1");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110111010011000);

  asm_cmd = build_c1_cmd("A", "D");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001100100000);

  asm_cmd = build_c1_cmd("AM", "A");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110000101000);

  asm_cmd = build_c1_cmd("AD", "!D");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001101110000);

  asm_cmd = build_c1_cmd("ADM", "!A");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110001111000);

  asm_cmd = build_c1_cmd("M", "-D");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001111001000);

  asm_cmd = build_c1_cmd("D", "-A");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110011010000);

  asm_cmd = build_c1_cmd("DM", "D+1");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110011111011000);

  asm_cmd = build_c1_cmd("A", "A+1");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110111100000);

  asm_cmd = build_c1_cmd("AM", "D-1");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001110101000);

  asm_cmd = build_c1_cmd("AD", "A-1");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110010110000);

  asm_cmd = build_c1_cmd("ADM", "D+A");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110000010111000);

  asm_cmd = build_c1_cmd("M", "D-A");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110010011001000);

  asm_cmd = build_c1_cmd("D", "A-D");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110000111010000);

  asm_cmd = build_c1_cmd("DM", "D&A");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110000000011000);

  asm_cmd = build_c1_cmd("A", "D|A");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110010101100000);

  asm_cmd = build_c1_cmd("AM", "M");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1111110000101000);

  asm_cmd = build_c1_cmd("AD", "!M");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1111110001110000);

  asm_cmd = build_c1_cmd("M", "-M");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1111110011001000);

  asm_cmd = build_c1_cmd("D", "M+1");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1111110111010000);

  asm_cmd = build_c1_cmd("DM", "M-1");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1111110010011000);

  asm_cmd = build_c1_cmd("A", "D+M");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1111000010100000);

  asm_cmd = build_c1_cmd("AM", "D-M");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1111010011101000);

  asm_cmd = build_c1_cmd("AD", "M-D");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1111000111110000);

  asm_cmd = build_c1_cmd("ADM", "D&M");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1111000000111000);

  asm_cmd = build_c1_cmd("ADM", "D|M");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1111010101111000);

  // C2 cmds

  asm_cmd = build_c2_cmd("0", "JGT");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110101010000001);

  asm_cmd = build_c2_cmd("1", "JEQ");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110111111000010);

  asm_cmd = build_c2_cmd("-1", "JGE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110111010000011);

  asm_cmd = build_c2_cmd("D", "JLT");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001100000100);

  asm_cmd = build_c2_cmd("A", "JNE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110000000101);

  asm_cmd = build_c2_cmd("!D", "JLE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001101000110);

  asm_cmd = build_c2_cmd("!A", "JMP");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110001000111);

  asm_cmd = build_c2_cmd("-D", "JGT");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001111000001);

  asm_cmd = build_c2_cmd("-A", "JEQ");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110011000010);

  asm_cmd = build_c2_cmd("D+1", "JGE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110011111000011);

  asm_cmd = build_c2_cmd("A+1", "JLT");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110111000100);

  asm_cmd = build_c2_cmd("D-1", "JNE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110001110000101);

  asm_cmd = build_c2_cmd("A-1", "JLE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110110010000110);

  asm_cmd = build_c2_cmd("D+A", "JMP");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110000010000111);

  asm_cmd = build_c2_cmd("D-A", "JGT");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110010011000001);

  asm_cmd = build_c2_cmd("A-D", "JEQ");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110000111000010);

  asm_cmd = build_c2_cmd("D&A", "JGE");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110000000000011);

  asm_cmd = build_c2_cmd("D|A", "JLT");
  bin_cmd = coder.code(asm_cmd);
  EXPECT_EQ(bin_cmd, 0b1110010101000100);

}
