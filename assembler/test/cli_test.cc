#include <gtest/gtest.h>
#include "error.hh"
#include "cli.hh"

class cli_test : public ::testing::Test
{
protected:
  void SetUp() override {}
  hackasm::cli cli;
  hackasm::cli::settings settings;
};
TEST_F(cli_test, short_help_flag) {
  const int test_argc = 2;
  char* test_argv[test_argc] = {"path", "-h"};
  settings = cli.parse(test_argc, test_argv);
  EXPECT_EQ(settings.help_bit, true);
}
TEST_F(cli_test, long_help_flag) {
  const int test_argc = 2;
  char* test_argv[test_argc] = {"path", "--help"};
  settings = cli.parse(test_argc, test_argv);
  EXPECT_EQ(settings.help_bit, true);
}
TEST_F(cli_test, long_format_bin) {
  const int test_argc = 4;
  char* test_argv[test_argc] = {"path", "--format", "bin", "infile"};
  settings = cli.parse(test_argc, test_argv);
  EXPECT_EQ(settings.out_fmt, hackasm::cli::HACKOUT_BINARY);
}
TEST_F(cli_test, long_format_txt) {
  const int test_argc = 4;
  char* test_argv[test_argc] = {"path", "--format", "txt", "infile"};
  settings = cli.parse(test_argc, test_argv);
  EXPECT_EQ(settings.out_fmt, hackasm::cli::HACKOUT_TEXT);
}
TEST_F(cli_test, long_format_invalid) {
  const int test_argc = 4;
  char* test_argv[test_argc] = {"path", "--format", "invalid_format", "infile"};
  EXPECT_THROW(settings = cli.parse(test_argc, test_argv), hackasm::unknown_out_format);
}
TEST_F(cli_test, long_format_missing_arg) {
  const int test_argc = 3;
  char* test_argv[test_argc] = {"path", "infile", "--format"};
  EXPECT_THROW(settings = cli.parse(test_argc, test_argv), hackasm::cli_missing_required_arg);
}
TEST_F(cli_test, short_format_bin) {
  const int test_argc = 4;
  char* test_argv[test_argc] = {"path", "-f", "bin", "infile"};
  settings = cli.parse(test_argc, test_argv);
  EXPECT_EQ(settings.out_fmt, hackasm::cli::HACKOUT_BINARY);
}
TEST_F(cli_test, short_format_txt) {
  const int test_argc = 4;
  char* test_argv[test_argc] = {"path", "-f", "txt", "infile"};
  settings = cli.parse(test_argc, test_argv);
  EXPECT_EQ(settings.out_fmt, hackasm::cli::HACKOUT_TEXT);
}
TEST_F(cli_test, short_format_invalid) {
  const int test_argc = 4;
  char* test_argv[test_argc] = {"path", "-f", "invalid_format", "infile"};
  EXPECT_THROW(settings = cli.parse(test_argc, test_argv), hackasm::unknown_out_format);
}
TEST_F(cli_test, short_format_missing_arg) {
  const int test_argc = 3;
  char* test_argv[test_argc] = {"path", "infile", "-f"};
  EXPECT_THROW(settings = cli.parse(test_argc, test_argv), hackasm::cli_missing_required_arg);
}
TEST_F(cli_test, long_out) {
  const int test_argc = 4;
  char* test_argv[test_argc] = {"path", "infile", "--out", "outfile"};
  settings = cli.parse(test_argc, test_argv);
  EXPECT_STREQ(settings.out_filename.c_str(), "outfile");
}
TEST_F(cli_test, long_out_missing_arg) {
  const int test_argc = 3;
  char* test_argv[test_argc] = {"path", "infile", "--out"};
  EXPECT_THROW(settings = cli.parse(test_argc, test_argv), hackasm::cli_missing_required_arg);
}
TEST_F(cli_test, short_out) {
  const int test_argc = 4;
  char* test_argv[test_argc] = {"path", "infile", "-o", "outfile"};
  settings = cli.parse(test_argc, test_argv);
  EXPECT_STREQ(settings.out_filename.c_str(), "outfile");
}
TEST_F(cli_test, short_out_missing_arg) {
  const int test_argc = 3;
  char* test_argv[test_argc] = {"path", "infile", "-o"};
  EXPECT_THROW(settings = cli.parse(test_argc, test_argv), hackasm::cli_missing_required_arg);
}
TEST_F(cli_test, infile) {
  const int test_argc = 2;
  char* test_argv[test_argc] = {"path", "infile"};
  settings = cli.parse(test_argc, test_argv);
  EXPECT_STREQ(settings.in_filename.c_str(), "infile");
}
TEST_F(cli_test, infile_missing) {
  const int test_argc = 1;
  char* test_argv[test_argc] = {"path"};
  EXPECT_THROW(settings = cli.parse(test_argc, test_argv), hackasm::cli_asm_file_arg_missing);
}
TEST_F(cli_test, unknown_long_option) {
  const int test_argc = 2;
  char* test_argv[test_argc] = {"path", "--unknown_long_option"};
  EXPECT_THROW(settings = cli.parse(test_argc, test_argv), hackasm::unknown_cli_option);
}
TEST_F(cli_test, unknown_short_option) {
  const int test_argc = 2;
  char* test_argv[test_argc] = {"path", "-u"};
  EXPECT_THROW(settings = cli.parse(test_argc, test_argv), hackasm::unknown_cli_option);
}
