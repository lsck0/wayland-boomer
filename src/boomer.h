#pragma once

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE   700

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <memory.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  char* window_title_boomermode;
  char* window_title_imagemode;
  int   window_width;
  int   window_height;
  float monitor_scaling;
  Color background_color;
  float pan_rigidity;
  float zoom_min;
  float zoom_max;
  float zoom_step;
  float flashlight_radius_min;
  float flashlight_radius_max;
  float flashlight_radius_step;
  float flashlight_radius_initial;
  float flashlight_radius_rigidity;
  float flashlight_darkness;
  float flashlight_darkness_rigidity;
  Color draw_color;
  float draw_thickness;
} Configuration;

typedef struct {
  char* program_name;
  char* screenshot_folder;
} Args;

typedef struct {
  Vector2 pan_current;
  Vector2 pan_target;
  float   zoom_current;
  float   zoom_target;
  bool    flashlight_rendering;
  bool    flashlight_enabled;
  bool    flashlight_enabled_previously;
  float   flashlight_radius_current;
  float   flashlight_radius_target;
  float   flashlight_darkness_current;
  bool    is_drawing;
  bool    is_mirrored;
} State;

extern Configuration g_default_configuration;
extern State         g_initial_state;

extern Configuration* g_configuration;
extern Args*          g_args;
extern State*         g_state;

void process_commandline_arguments(int argc, char** argv);

Image load_image_from_stdin(bool* out_was_file);

void handle_inputs(void);
void handle_draw(void);
void lines_draw(void);
void lines_clear(void);
