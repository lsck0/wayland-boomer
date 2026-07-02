#include "boomer.h"

#ifndef VERSION
#define VERSION "unknown"
#endif
static const char* s_version = VERSION;

#define shift(argc, argv)                                                                                                                            \
  do {                                                                                                                                               \
    (argc)--;                                                                                                                                        \
    (argv)++;                                                                                                                                        \
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
    if (strcmp(*argv, "-ms") == 0 || strcmp(*argv, "--monitor-scaling") == 0) {
      shift(argc, argv);
      if (argc < 1) { error_and_exit("Missing value for -mc/--monitor-scaling."); }

      errno              = 0;
      float parse_result = strtof(*argv, NULL);
      if (errno != 0 || parse_result <= 0.0F) {
        error_and_exit("Invalid value for -mc/--monitor-scaling. It has to be a positive floating point number.");
      }

      g_configuration->monitor_scaling = parse_result;

      shift(argc, argv);
      continue;
    }
    if (strcmp(*argv, "-sd") == 0 || strcmp(*argv, "--screenshot-dir") == 0) {
      shift(argc, argv);
      if (argc < 1) { error_and_exit("Missing value for -sd/--screenshot-dir."); }

      if (!DirectoryExists(*argv)) { error_and_exit("Invalid value for -sd/--screenshot-dir. The directory does not exist."); }

      g_args->screenshot_folder = *argv;

      shift(argc, argv);
      continue;
    }
    if (strcmp(*argv, "-bg") == 0 || strcmp(*argv, "--background") == 0) {
      shift(argc, argv);
      if (argc < 1) { error_and_exit("Missing value for -bg/--background."); }

      unsigned long parse_result = strtoul(*argv, NULL, 16);
      if (parse_result < 0 || parse_result > 0xFFFFFFFF || strlen(*argv) != 8) {
        error_and_exit("Invalid value for -bg/--background. It has to be a hex value of form RRGGBBAA.");
      }

      g_configuration->background_color = (Color){
        .r = (unsigned char)((parse_result >> 24) & 0xFF),
        .g = (unsigned char)((parse_result >> 16) & 0xFF),
        .b = (unsigned char)((parse_result >> 8) & 0xFF),
        .a = (unsigned char)(parse_result & 0xFF),
      };

      shift(argc, argv);
      continue;
    }
    if (strcmp(*argv, "-pr") == 0 || strcmp(*argv, "--pan-rigidity") == 0) {
      shift(argc, argv);
      if (argc < 1) { error_and_exit("Missing value for -pr/--pan-rigidity."); }
      errno              = 0;
      float parse_result = strtof(*argv, NULL);
      if (errno != 0 || parse_result <= 0.0F) {
        error_and_exit("Invalid value for -pr/--pan-rigidity. It has to be a positive floating point number.");
      }

      g_configuration->pan_rigidity = parse_result;

      shift(argc, argv);
      continue;
    }
    if (strcmp(*argv, "-fr") == 0 || strcmp(*argv, "--flashlight-rigidity") == 0) {
      shift(argc, argv);
      if (argc < 1) { error_and_exit("Missing value for -fr/--flashlight-rigidity."); }

      errno              = 0;
      float parse_result = strtof(*argv, NULL);
      if (errno != 0 || parse_result <= 0.0F) {
        error_and_exit("Invalid value for -fr/--flashlight-rigidity. It has to be a positive floating point number.");
      }

      g_configuration->flashlight_radius_rigidity = parse_result;

      shift(argc, argv);
      continue;
    }
    if (strcmp(*argv, "-fd") == 0 || strcmp(*argv, "--flashlight-darkness") == 0) {
      shift(argc, argv);
      if (argc < 1) { error_and_exit("Missing value for -fd/--flashlight-darkness."); }

      errno              = 0;
      float parse_result = strtof(*argv, NULL);
      if (errno != 0 || parse_result < 0.0F || parse_result > 1.0F) {
        error_and_exit("Invalid value for -fd/--flashlight-darkness. It has to be a floating point number between 0 and 1.");
      }

      g_configuration->flashlight_darkness = parse_result;

      shift(argc, argv);
      continue;
    }
  }
}

static void print_usage(FILE* sink) {
  assert(sink != NULL);

  unsigned long default_bg_color_hex = (g_configuration->background_color.r << 24) | (g_configuration->background_color.g << 16) |
                                       (g_configuration->background_color.b << 8) | g_configuration->background_color.a;

  // clang-format off
  (void)fprintf(sink, "Usage: \n");
  (void)fprintf(sink, "  grim - | %s [options]                                      Boomer Mode\n", g_args->program_name);
  (void)fprintf(sink, "  %s [options] < image.[png|jpg|webp|bmp]                    Image Viewer Mode\n", g_args->program_name);
  (void)fprintf(sink, "Options:\n");
  (void)fprintf(sink, "  -h,              --help                                 %*s Show this message and exit.\n", (int)strlen(g_args->program_name), " ");
  (void)fprintf(sink, "  -v,              --version                              %*s Show version and exit.\n", (int)strlen(g_args->program_name), " ");
  (void)fprintf(sink, "  -ms <float>,     --monitor-scaling <float>              %*s Compositor monitor scaling (default %lf).\n", (int)strlen(g_args->program_name), " ", g_configuration->monitor_scaling);
  (void)fprintf(sink, "  -sd <path>,      --screenshot-dir <path>                %*s Folder to save screenshots in.\n", (int)strlen(g_args->program_name), " ");
  (void)fprintf(sink, "  -bg <rgba hex>,  --background <rgba hex>                %*s Background color (default %.8lx).\n", (int)strlen(g_args->program_name), " ", default_bg_color_hex);
  (void)fprintf(sink, "  -pr <float>,     --pan-rigidity <float>                 %*s Pan/Zoom rigidity (default %lf).\n", (int)strlen(g_args->program_name), " ", g_configuration->pan_rigidity);
  (void)fprintf(sink, "  -fr <float>,     --flashlight-rigidity <float>          %*s Flashlight radius rigidity (default %lf).\n", (int)strlen(g_args->program_name), " ", g_configuration->flashlight_radius_rigidity);
  (void)fprintf(sink, "  -fd <float>,     --flashlight-darkness <float>          %*s Flashlight darkness (default %lf).\n", (int)strlen(g_args->program_name), " ", g_configuration->flashlight_darkness);
  (void)fprintf(sink, "  -fdr <float>,    --flashlight-darkness-rigidity <float> %*s Flashlight darkness rigidity (default %lf).\n", (int)strlen(g_args->program_name), " ", g_configuration->flashlight_darkness_rigidity);
  // clang-format on
}

static noreturn void error_and_exit(const char* message) {
  (void)fprintf(stderr, "Error: %s\n\n", message);
  print_usage(stderr);

  exit(EXIT_FAILURE);
}
