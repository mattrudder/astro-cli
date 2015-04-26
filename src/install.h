#include <astro/logging.h>
#include <astro/io/file.h>

namespace astro
{
  using namespace io;

  inline bool
  install_packages(int argc, const char** argv)
  {
    if (argc > 0)
    {

    }
    else
    {
      // TODO: Install dependencies from current package.json file.
      if (file::exists("package.json"))
      {
        auto pkg = package_from_file("package.json");

        log_info("Updating package %s", pkg.name);

        auto deps = pkg.dependencies;
        for (uintptr i = 0; i < pkg.dependency_count; ++i, ++deps)
          log_info("Found dependency %s, version %s",
            deps->name, version_to_string(deps->version));
      }
    }

    return false;
  }
}
