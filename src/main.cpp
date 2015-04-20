#include <stdio.h>
#include <git2.h>

extern "C"
{
#include <commander/commander.h>
#include <commander/commander.c>
}

struct options_t
{
};

int main(int argc, char *argv[])
{
  options_t options;
  command_t cmd = {};
  command_init(&cmd, argv[0], "0.0.1"); // TODO: Read version from package.json?
  // command_option(&cmd, "-v", "--verbose", "enable verbose stuff", verbose);
  // command_option(&cmd, "-r", "--required <arg>", "required arg", required);
  // command_option(&cmd, "-o", "--optional [arg]", "optional arg", optional);

  cmd.data = &options;
  command_parse(&cmd, argc, argv);

  git_libgit2_init();

  git_config *cfg = NULL;
  int error = git_config_open_default(&cfg);

  git_libgit2_shutdown();
  return 0;
}
