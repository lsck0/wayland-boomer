#include "./headers/args.h"

#include "./headers/globals.h"

#include <assert.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#define shift(argc, argv)                                                                                              \
  do {                                                                                                                 \
    argc--;                                                                                                            \
    argv++;                                                                                                            \
  } while (0)

static void          print_usage(void);
static noreturn void error_and_exit(const char* message);

void process_commandline_arguments(int argc, char** argv) {
  assert(argc > 0);
  g_args->program_name = argv[0];

  while (argc > 1) {
    shift(argc, argv);

    // TODO: flag parsing
  }
}

static void print_usage(void) {
}

static noreturn void error_and_exit(const char* message) {
  (void)message;

  abort();
}
