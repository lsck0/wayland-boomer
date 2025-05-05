#include "./headers/controls.h"

#include "./headers/defaults.h"
#include "./headers/state.h"

#include <raymath.h>

static void handle_reset(State* state);
static void handle_panning(State* state);
static void handle_zoom(State* state);

void handle_inputs(State* state) {
  handle_reset(state);
  handle_panning(state);
  handle_zoom(state);
}

static void handle_reset(State* state) {
  if (IsKeyPressed(KEY_ZERO)) {
    *state = initial_wb_state;
  }
}

static void handle_panning(State* state) {
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    // BUG: raylib's GetMousePosition() is broken when the window is on multiple monitors
    Vector2 d     = GetMouseDelta();
    state->pan.x += d.x;
    state->pan.y += d.y;

    // Vector2 mouse_pos = GetMousePosition();
    // TraceLog(LOG_INFO, "Mouse position: %f %f", mouse_pos.x, mouse_pos.y);
  }
}

static void handle_zoom(State* state) {
  float wheel = GetMouseWheelMove();
  if (wheel != 0) {
    // BUG: raylib's GetMousePosition() is broken when the window is on multiple monitors
    Vector2 mouse_pos = GetMousePosition();
    float   prev_zoom = state->zoom;
    Vector2 world     = {(mouse_pos.x - state->pan.x) / prev_zoom, (mouse_pos.y - state->pan.y) / prev_zoom};
    state->zoom       = Clamp(state->zoom + wheel * ZOOM_STEP, ZOOM_MIN, ZOOM_MAX);
    state->pan.x      = mouse_pos.x - world.x * state->zoom;
    state->pan.y      = mouse_pos.y - world.y * state->zoom;
  }
}
