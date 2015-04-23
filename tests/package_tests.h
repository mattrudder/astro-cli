#include "package.h"

const char* pkg_str = "{\n"
  "  \"name\": \"astro-cli\",\n"
  "  \"version\": \"1.2.3\",\n"
  "  \"repo\": \"team-astro/astro-cli\",\n"
  "  \"description\": \"Project generation for Astro\",\n"
  "  \"keywords\": [\"astro\", \"generator\"],\n"
  "  \"license\": \"MIT\",\n"
  "  \"install\": \"make install\",\n"
  "  \"dependencies\": {\n"
  "    \"clibs/commander\": \"1.3.2\",\n"
  "    \"jwerle/fs.c\": \"0.1.2\",\n"
  "    \"thlorenz/log.h\": \"0.3.0\"\n"
  "  }\n"
  "}";

#define ASSERT_VERSION_EQ(x, y, z, v) \
  ASSERT_EQ(x, v.major);  \
  ASSERT_EQ(y, v.minor);  \
  ASSERT_EQ(z, v.patch);

TEST package_should_parse()
{
  astro::package pkg = astro::package_from_json(pkg_str);
  ASSERT_STR_EQ("astro-cli", pkg.name);
  ASSERT_STR_EQ("team-astro/astro-cli", pkg.repo);
  ASSERT_VERSION_EQ(1, 2, 3, pkg.version)

  ASSERT_EQ(3, pkg.dependency_count);

  ASSERT_STR_EQ("clibs/commander", pkg.dependencies[0].name);
  ASSERT_STR_EQ("jwerle/fs.c", pkg.dependencies[1].name);
  ASSERT_STR_EQ("thlorenz/log.h", pkg.dependencies[2].name);

  ASSERT_VERSION_EQ(1, 3, 2, pkg.dependencies[0].version);
  ASSERT_VERSION_EQ(0, 1, 2, pkg.dependencies[1].version);
  ASSERT_VERSION_EQ(0, 3, 0, pkg.dependencies[2].version);

  PASS();
}

TEST package_ref_should_parse()
{
  astro::package_ref ref = astro::package_ref_parse("foo@1.2.3");
  ASSERT_STR_EQ("foo", ref.name);
  ASSERT_VERSION_EQ(1, 2, 3, ref.version);

  ref = astro::package_ref_parse("bar");
  ASSERT_STR_EQ("bar", ref.name);
  ASSERT_VERSION_EQ(0, 0, 0, ref.version);

  PASS();
}

SUITE(package_tests)
{
  // SET_SETUP(setup_cb, voidp_to_callback_data);
  // SET_TEARDOWN(teardown_cb, voidp_to_callback_data);

  RUN_TEST(package_should_parse);
  RUN_TEST(package_ref_should_parse);
}
