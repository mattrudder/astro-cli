#include <json11/json11.hpp>
#include <astro/astro.h>
#include <astro/memory.h>

extern "C"
{
#include <fs/fs.h>
}

namespace astro
{
  struct package
  {
    const char* name;
    const char* version;
    const char* repo;

    static package from_file(const char* path);
    static package from_string(const char* str);
  };

  package package::from_file(const char* path)
  {
    memory_pool pool = {};
    package result = {};
    if (fs_exists(path))
    {
      char* str = fs_read(path);
      result = package::from_string(str);
      free(str);
    }

    return result;
  }

  package package::from_string(const char* str)
  {
    package result = {};

    std::string err;
    auto json = json11::Json::parse(str, err);

    result.name = strdup(json["name"].string_value().c_str());
    result.version = strdup(json["version"].string_value().c_str());
    result.repo = strdup(json["repo"].string_value().c_str());

    return result;
  }
}
