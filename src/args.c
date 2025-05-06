#include "./headers/args.h"

#include <assert.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#define shift(argc, argv)                                                                                              \
  do {                                                                                                                 \
    argc--;                                                                                                            \
    argv++;                                                                                                            \
  } while (0)

static void          print_usage(const char* program_name);
static noreturn void error_and_exit(const char* program_name, const char* message);

Args parse_commandline_arguments(int argc, char** argv) {
  Args args = {0};

  assert(argc > 0);
  args.program_name = argv[0];

  while (argc > 1) {
    shift(argc, argv);

    // TODO: flag parsing
  }

  return args;
}

static void print_usage(const char* program_name) {
  (void)program_name;
}

static noreturn void error_and_exit(const char* program_name, const char* message) {
  (void)program_name;
  (void)message;

  abort();
}
