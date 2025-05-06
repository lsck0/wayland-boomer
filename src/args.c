#include "./headers/args.h"

#include "./headers/globals.h"

#include <assert.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#ifndef VERSION
#define VERSION "unknown"
#endif
static const char* s_version = VERSION;

#define shift(argc, argv)                                                                                              \
  do {                                                                                                                 \
    (argc)--;                                                                                                          \
    (argv)++;                                                                                                          \
  } while (0)

static void          print_usage(FILE* sink);
static noreturn void error_and_exit(const char* message);

void process_commandline_arguments(int argc, char** argv) {
  assert(argc > 0);
  g_args->program_name = argv[0];

  while (argc > 1) {
    shift(argc, argv);

    if (strcmp(*argv, "-h") == 0 || strcmp(*argv, "--help") == 0) {
      print_usage(stdout);
      exit(EXIT_SUCCESS);
    }
    if (strcmp(*argv, "-v") == 0 || strcmp(*argv, "--version") == 0) {
      (void)fprintf(stdout, "Version: %s\n", s_version);
      exit(EXIT_SUCCESS);
    }
    if (strcmp(*argv, "-lmm") == 0 || strcmp(*argv, "--leftmost-monitor") == 0) {
      shift(argc, argv);
      if (argc < 1) {
        error_and_exit("Missing value for -lmm/--leftmost-monitor.");
      }

      long int parse_result = strtol(*argv, NULL, 10);
      if (parse_result < 0 || parse_result >= 5) { // surely noone sane has 5+ monitors
        error_and_exit(
            "Invalid value for -lmm/--leftmost-monitor. It has to be a number between 0 and your monitor count -1."
        );
      }
      g_args->leftmost_monitor = (int)parse_result;

      shift(argc, argv);
      continue;
    }
    if (strcmp(*argv, "-sd") == 0 || strcmp(*argv, "--screenshot-dir") == 0) {
      shift(argc, argv);
      if (argc < 1) {
        error_and_exit("Missing value for -sd/--screenshot-dir.");
      }

      if (!DirectoryExists(*argv)) {
        error_and_exit("Invalid value for -sd/--screenshot-dir. The directory does not exist.");
      }

      g_args->screenshot_folder = *argv;

      shift(argc, argv);
      continue;
    }
  }
}

static void print_usage(FILE* sink) {
  assert(sink != NULL);

  // clang-format off
  (void)fprintf(sink, "Usage: \n");
  (void)fprintf(sink, "  grim - | %s [options]                    Boomer Mode\n", g_args->program_name);
  (void)fprintf(sink, "  %s [options] < image.[png|jpg|webp|bmp]  Image Viewer Mode\n", g_args->program_name);
  (void)fprintf(sink, "Options:\n");
  (void)fprintf(sink, "  -h,         --help                    %*s Show this message and exit.\n", (int)strlen(g_args->program_name), " ");
  (void)fprintf(sink, "  -v,         --version                 %*s Show version and exit.\n", (int)strlen(g_args->program_name), " ");
  (void)fprintf(sink, "  -lmm <int>, --leftmost-monitor <int>  %*s Monitor to place the window origin on.\n", (int)strlen(g_args->program_name), " ");
  (void)fprintf(sink, "  -sd <path>, --screenshot-dir <path>   %*s Folder to save screenshots in.\n", (int)strlen(g_args->program_name), " ");
  // clang-format on
}

static noreturn void error_and_exit(const char* message) {
  (void)fprintf(stderr, "Error: %s\n\n", message);
  print_usage(stderr);

  exit(EXIT_FAILURE);
}
