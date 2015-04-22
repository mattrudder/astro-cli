#include <json11/json11.hpp>
#include <astro/astro.h>
#include <astro/memory.h>
#include <astro/string.h>

extern "C"
{
#include <fs/fs.h>
}

#include "version.h"

namespace astro
{
  struct dependency
  {
    const char* name;
    version version;
  };

  struct package
  {
    const char* name;
    const char* repo;
    version version;

    uint16 dependency_count;
    dependency* dependencies;

    static package from_file(const char* path, allocator alloc = allocator::malloc());
    static package from_string(const char* str, allocator alloc = allocator::malloc());
  };

  package package::from_file(const char* path, allocator alloc)
  {
    package result = {};
    if (!fs_exists(path))
    {
      char* str = fs_read(path);
      result = package::from_string(str, alloc);
      free(str);
    }

    return result;
  }

  package package::from_string(const char* str, allocator alloc)
  {
    package result = {};

    std::string err;
    auto json = json11::Json::parse(str, err);

    result.name = strdup(json["name"].string_value().c_str(), alloc);
    result.repo = strdup(json["repo"].string_value().c_str(), alloc);

    const char* ver = strdup(json["version"].string_value().c_str(), alloc);
    result.version = version::parse(ver);

    auto deps = json["dependencies"].object_items();
    result.dependency_count = deps.size();
    result.dependencies = (dependency*) alloc.allocate(result.dependency_count * sizeof(dependency));

    dependency* dep = result.dependencies;
    for (auto&& d : deps)
    {
      dep->name = strdup(d.first.c_str(), alloc);
      dep->version = version::parse(d.second.string_value().c_str());

      ++dep;
    }

    return result;
  }
}
