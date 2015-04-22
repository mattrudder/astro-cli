#include <stdio.h>
#include <git2.h>
#include <curl/curl.h>
#include <log.h/log.h>

#include <json11/json11.cpp>
extern "C"
{
#include <commander/commander.c>
#include <fs/fs.c>
}

#include <astro/string.h>
#include "package.h"
using namespace astro;

struct options_t
{
};

int main(int argc, char *argv[])
{
  options_t options;
  command_t cmd = {};
  command_init(&cmd, argv[0], ASTRO_CLI_VERSION);
  // command_option(&cmd, "-v", "--verbose", "enable verbose stuff", verbose);
  // command_option(&cmd, "-r", "--required <arg>", "required arg", required);
  // command_option(&cmd, "-o", "--optional [arg]", "optional arg", optional);

  cmd.data = &options;
  command_parse(&cmd, argc, argv);

  if (!fs_exists("package.json"))
  {
    auto pkg = package::from_file("package.json");

    log_info("Updating package %s", pkg.name);

    auto deps = pkg.dependencies;
    for (uintptr i = 0; i < pkg.dependency_count; ++i, ++deps)
      log_info("Found dependency %s, version %s",
        deps->name, version::to_string(deps->version));

  }

  curl_global_init(CURL_GLOBAL_DEFAULT);
  git_libgit2_init();

  git_config *cfg = NULL;
  int error = git_config_open_default(&cfg);

  git_libgit2_shutdown();
  curl_global_cleanup();
  return 0;
}
