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
  struct package_ref
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
    package_ref* dependencies;
  };

  static constexpr char package_ref_separator = '@';

  inline package_ref
  package_ref_parse(const char* str, allocator alloc = allocator::malloc())
  {
    package_ref result = {};
    if (str && strlen(str) > 0)
    {
      const char* version = strchr(str, package_ref_separator);
      if (version)
      {
        version += 1;
        size_t name_len = strlen(str) - strlen(version);
        result.version = version_parse(version);
        char* name = (char*)alloc.allocate(name_len);
        strncpy(name, str, name_len);
        name[name_len - 1] = '\0';
        result.name = name;
      }
      else
      {
        result.name = strdup(str, alloc);
      }
    }

    return result;
  }

  inline package
  package_from_json(const char* str, allocator alloc = allocator::malloc())
  {
    package result = {};

    std::string err;
    auto json = json11::Json::parse(str, err);

    result.name = strdup(json["name"].string_value().c_str(), alloc);
    result.repo = strdup(json["repo"].string_value().c_str(), alloc);

    const char* ver = strdup(json["version"].string_value().c_str(), alloc);
    result.version = version_parse(ver);

    auto deps = json["dependencies"].object_items();
    result.dependency_count = deps.size();
    result.dependencies = (package_ref*) alloc.allocate(result.dependency_count * sizeof(package_ref));

    package_ref* ref = result.dependencies;
    for (auto&& d : deps)
    {
      ref->name = strdup(d.first.c_str(), alloc);
      ref->version = version_parse(d.second.string_value().c_str());

      ++ref;
    }

    return result;
  }

  inline package
  package_from_file(const char* path, allocator alloc = allocator::malloc())
  {
    package result = {};
    if (!fs_exists(path))
    {
      char* str = fs_read(path);
      result = package_from_json(str, alloc);
      free(str);
    }

    return result;
  }
}
