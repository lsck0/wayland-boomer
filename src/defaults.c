#include "./headers/defaults.h"

// TODO: make them overwritable via flags
const char* WINDOW_TITLE     = "wayland-boomer";
const int   WINDOW_WIDTH     = 1080;
const int   WINDOW_HEIGHT    = 720;
const float ZOOM_MIN         = 0.01F;
const float ZOOM_MAX         = 20.0F;
const float ZOOM_STEP        = 0.2F;
const Color BACKGROUND_COLOR = BLACK;

const State initial_wb_state = {
    .pan  = {0, 0},
    .zoom = 1.0F,
};
