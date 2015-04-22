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

TEST package_should_parse()
{
  astro::package pkg = astro::package::from_string(pkg_str);
  ASSERT_STR_EQ("astro-cli", pkg.name);
  ASSERT_STR_EQ("team-astro/astro-cli", pkg.repo);
  ASSERT_EQ(1, pkg.version.major);
  ASSERT_EQ(2, pkg.version.minor);
  ASSERT_EQ(3, pkg.version.patch);

  ASSERT_EQ(3, pkg.dependency_count);

  PASS();
}

SUITE(package_tests)
{
  // SET_SETUP(setup_cb, voidp_to_callback_data);
  // SET_TEARDOWN(teardown_cb, voidp_to_callback_data);

  RUN_TEST(package_should_parse);
}
