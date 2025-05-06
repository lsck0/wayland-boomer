#include "./headers/controls.h"

#include "./headers/globals.h"

#include <raymath.h>

static void handle_reset(void);
static void handle_panning(void);
static void handle_zoom(void);
static void handle_flashlight(void);

void handle_inputs(void) {
  handle_reset();
  handle_panning();
  handle_zoom();
  handle_flashlight();
}

static void handle_reset(void) {
  if (IsKeyPressed(KEY_ZERO)) {
    *g_state = g_initial_state;
  }
}

static void handle_panning(void) {
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    // BUG: raylib's GetMousePosition() is broken when the window is on multiple monitors
    Vector2 mouse_delta  = GetMouseDelta();
    g_state->pan.x      += mouse_delta.x;
    g_state->pan.y      += mouse_delta.y;

    // Vector2 mouse_pos = GetMousePosition();
    // TraceLog(LOG_INFO, "Mouse position: %f %f", mouse_pos.x, mouse_pos.y);
  }
}

static void handle_zoom(void) {
  float wheel = GetMouseWheelMove();
  if (wheel != 0 && !IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_RIGHT_CONTROL)) {
    // BUG: raylib's GetMousePosition() is broken when the window is on multiple monitors
    Vector2 mouse_pos = GetMousePosition();
    float   prev_zoom = g_state->zoom;
    Vector2 world     = {(mouse_pos.x - g_state->pan.x) / prev_zoom, (mouse_pos.y - g_state->pan.y) / prev_zoom};
    g_state->zoom =
        Clamp(g_state->zoom + wheel * g_configuration->zoom_step, g_configuration->zoom_min, g_configuration->zoom_max);
    g_state->pan.x = mouse_pos.x - world.x * g_state->zoom;
    g_state->pan.y = mouse_pos.y - world.y * g_state->zoom;
  }
}

static void handle_flashlight(void) {
  if (IsKeyPressed(KEY_F)) {
    g_state->flashlight_enabled = !g_state->flashlight_enabled;
  }

  float wheel = GetMouseWheelMove();
  if (g_state->flashlight_enabled && wheel != 0) {
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
      g_state->flashlight_radius -= wheel * g_configuration->flashlight_radius_step;
      g_state->flashlight_radius  = Clamp(
          g_state->flashlight_radius,
          g_configuration->flashlight_radius_min,
          g_configuration->flashlight_radius_max
      );
    }
  }
}
