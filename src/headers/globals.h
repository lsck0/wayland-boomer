#pragma once

#include <raylib.h>

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

typedef struct {
  Vector2 pan;
  float   zoom;
  bool    flashlight_enabled;
  float   flashlight_radius;
  bool    is_drawing;
} State;

typedef struct {
  char* program_name;
  int   leftmost_monitor;
  char* screenshot_folder;
} Args;

typedef struct {
  char* window_title;
  int   window_width;
  int   window_height;
  Color background_color;
  float zoom_min;
  float zoom_max;
  float zoom_step;
  float flashlight_radius_min;
  float flashlight_radius_max;
  float flashlight_radius_step;
  Color draw_color;
  float draw_thickness;
} Configuration;

extern const Configuration g_default_configuration;
extern const State         g_initial_state;

extern Configuration* g_configuration;
extern Args*          g_args;
extern State*         g_state;

#endif // __GLOBALS_H__
