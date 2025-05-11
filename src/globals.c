#include "./headers/globals.h"

#include "./headers/draw.h"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

const Configuration g_default_configuration = {
    .window_title_boomermode = "wayland-boomer",
    .window_title_imagemode  = "wayland-boomer - image viewer",
    .window_width            = 1080,
    .window_height           = 720,
    .background_color        = BLACK,
    .zoom_min                = 0.25F,
    .zoom_max                = 20.0F,
    .zoom_step               = 0.2F,
    .flashlight_radius_min   = 20.0F,
    .flashlight_radius_max   = 600.0F,
    .flashlight_radius_step  = 20.0F,
    .draw_color              = RED,
    .draw_thickness          = 3.5F,
};

const Args g_default_args = {
    .program_name      = NULL,
    .screenshot_folder = NULL,
};

const State g_initial_state = {
    .pan                = {0, 0},
    .zoom               = 1.0F,
    .flashlight_enabled = false,
    .flashlight_radius  = 100.0F,
    .is_drawing         = false,
};

Configuration* g_configuration = NULL;
Args*          g_args          = NULL;
State*         g_state         = NULL;

__attribute__((__constructor__)) void initialize_globals(void) {
  g_configuration = malloc(sizeof(Configuration));
  assert(g_configuration);
  *g_configuration = g_default_configuration;

  g_args = malloc(sizeof(Args));
  assert(g_args);
  *g_args = g_default_args;

  g_state = malloc(sizeof(State));
  assert(g_state);
  *g_state = g_initial_state;
}

__attribute__((__destructor__)) void deinitialize_globals(void) {
  free(g_configuration);
  free(g_args);
  free(g_state);
  lines_clear();
}
