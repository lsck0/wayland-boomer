#include "./headers/defaults.h"

// TODO: make them overwritable via flags
const char* WINDOW_TITLE           = "wayland-boomer";
const int   WINDOW_WIDTH           = 1080;
const int   WINDOW_HEIGHT          = 720;
const Color BACKGROUND_COLOR       = BLACK;
const float ZOOM_MIN               = 0.01F;
const float ZOOM_MAX               = 20.0F;
const float ZOOM_STEP              = 0.2F;
const float FLASHLIGHT_RADIUS_MIN  = 20.0F;
const float FLASHLIGHT_RADIUS_MAX  = 600.0F;
const float FLASHLIGHT_RADIUS_STEP = 20.0F;

const State initial_state = {
    .pan                = {0, 0},
    .zoom               = 1.0F,
    .flashlight_enabled = false,
    .flashlight_radius  = 100.0F,
};
