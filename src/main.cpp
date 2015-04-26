#include <stdio.h>
#include <git2.h>
#include <curl/curl.h>
#include <json11/json11.cpp>

#include <astro/astro.h>
#include <astro/string.h>
#include "package.h"
#include "install.h"
using namespace astro;
using namespace std;
using namespace std::placeholders;

astro::log_level astro_log_verbosity = astro::log_level::debug;

bool
print_usage(int argc, const char** argv, bool print_description);

struct options_t
{
};

struct command
{
  const char* name;
  const char* description;
  std::function<int (int, const char **)> handler;
  const char* short_name;
  bool option;
};

command commands[] =
{
  { "help", "", std::bind(&print_usage, _1, _2, false), nullptr },
  { nullptr, "", std::bind(&print_usage, _1, _2, true), "-l" },
  { "install", "Install packages", install_packages, nullptr },
};

void print_library_dependencies()
{
  auto curl_version = curl_version_info(CURLVERSION_NOW);
  log_debug("libcurl %s (%s) %s libz/%s",
    curl_version->version, curl_version->host, curl_version->ssl_version,
    curl_version->libz_version);

  int git_version[3];
  git_libgit2_version(git_version, git_version+1, git_version+2);
  log_debug("libgit2 %d.%d.%d",
    git_version[0], git_version[1], git_version[2]);
}

bool
print_usage(int argc, const char** argv, bool print_description = false)
{
  fprintf(stderr,
    "Usage: astro <command>\n"
    "\n"
    "where <command> is one of:\n");

  int line_len = 0;
  command* cmd = commands;
  size_t command_len = ASTRO_COUNTOF(commands);
  for (size_t i = 0; i < command_len; ++i, ++cmd)
  {
    if (!cmd->name)
      continue;

    if (print_description)
    {
      fprintf(stderr,
        "\n%s\t%s", cmd->name, cmd->description);
    }
    else
    {
      const char* prefix = i == 0 ? "  " : "";
      if (line_len >= 80)
      {
        line_len = 0;
        prefix = "\n  ";
      }

      line_len += fprintf(stderr,
        "%s%s%s",
        prefix,
        cmd->name,
        (i + 1) < command_len ? ", " : "");
    }

  }

  fprintf(stderr,
    "\n\n"
    "astro <command> -h  quick help on <command>\n"
    "\n"
    "astro@%s\n", ASTRO_CLI_VERSION);

  return false;
}

int main(int argc, const char** argv)
{
  argc -= 1;
  argv += 1;

  if (argc < 1 || !argv[0] || strcmp("help", argv[0]) == 0)
  {
    print_usage(argc, argv);
    return 0;
  }
  else if (strcmp("-v", argv[0]) == 0) // TODO: Handle this anywhere in the arg list.
  {
    astro_log_verbosity = log_level::debug;
  }

  curl_global_init(CURL_GLOBAL_ALL);
  git_libgit2_init();

  print_library_dependencies();

  git_config *cfg = NULL;
  int error = git_config_open_default(&cfg);

  command* running_cmd = nullptr;
  size_t command_len = ASTRO_COUNTOF(commands);
  for (size_t i = 0; i < command_len && argc > 0; ++i)
  {
    command* cmd = commands + i;
    if ((cmd->name && strcmp(cmd->name, argv[0]) == 0)
      || (cmd->short_name && strcmp(cmd->short_name, argv[0]) == 0))
    {
      argc -= 1;
      argv += 1;

      running_cmd = commands + i;
      if (!running_cmd->handler(argc, argv))
      {
        break;
      }
    }
  }

  git_libgit2_shutdown();
  curl_global_cleanup();

  if (!running_cmd)
  {
    print_usage(argc, argv);
    return 1;
  }

  return 0;
}
