#include "boomer.h"

Configuration g_default_configuration = {
  .window_title_boomermode    = "wayland-boomer",
  .window_title_imagemode     = "wayland-boomer - image viewer",
  .window_width               = 1080,
  .window_height              = 720,
  .monitor_scaling            = 1.0F,
  .background_color           = BLACK,
  .pan_rigidity               = 25.0F,
  .zoom_min                   = 0.25F,
  .zoom_max                   = 20.0F,
  .zoom_step                  = 0.2F,
  .flashlight_radius_min      = 20.0F,
  .flashlight_radius_max      = 600.0F,
  .flashlight_radius_step     = 30.0F,
  .flashlight_radius_initial  = 100.0F,
  .flashlight_darkness        = 0.1F,
  .flashlight_radius_rigidity = 10.0F,
  .draw_color                 = RED,
  .draw_thickness             = 3.5F,
};

static Args g_default_args = {
  .program_name      = NULL,
  .screenshot_folder = NULL,
};

State g_initial_state = {
  .pan_current          = { 0, 0 },
  .pan_target           = { 0, 0 },
  .zoom_current         = 1.0F,
  .zoom_target          = 1.0F,
  .flashlight_rendering = false,
  .flashlight_enabled   = false,
  .is_drawing           = false,
};

Configuration* g_configuration = NULL;
Args*          g_args          = NULL;
State*         g_state         = NULL;

__attribute__((__constructor__)) static void initialize_globals(void) {
  g_initial_state.flashlight_radius_current   = g_default_configuration.flashlight_radius_initial;
  g_initial_state.flashlight_radius_target    = g_default_configuration.flashlight_radius_initial;
  g_initial_state.flashlight_darkness_current = g_default_configuration.flashlight_darkness;

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

__attribute__((__destructor__)) static void deinitialize_globals(void) {
  free(g_configuration);
  free(g_args);
  free(g_state);
  lines_clear();
}
