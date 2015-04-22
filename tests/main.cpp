#include <greatest/greatest.h>
#include <json11/json11.cpp>
extern "C"
{
#include <commander/commander.c>
#include <fs/fs.c>
}

#include "package_tests.h"

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

/* main(), for a standalone command-line test runner.
 * This replaces run_tests above, and adds command line option
 * handling and exiting with a pass/fail status. */
int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
    RUN_SUITE(package_tests);
    GREATEST_MAIN_END();        /* display results */
}
