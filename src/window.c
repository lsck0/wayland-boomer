#include "./headers/window.h"

#include "./headers/globals.h"

#include <assert.h>
#include <raylib.h>

static void place_window_top_left(void);

void init_raylib_window(int width, int height, bool image_was_file) {
  assert(width > 0);
  assert(height > 0);

  if (image_was_file) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    width  = g_configuration->window_width;
    height = g_configuration->window_height;
  } else {
    SetConfigFlags(
        FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_RESIZABLE
    );
  }

  InitWindow(width, height, g_configuration->window_title);
  SetWindowFocused();

  if (!image_was_file) place_window_top_left();
}

static void place_window_top_left(void) {
  TraceLog(LOG_INFO, "Monitor count: %d", GetMonitorCount());
  for (int monitor_idx = 0; monitor_idx < GetMonitorCount(); monitor_idx++) {
    Vector2 monitor_position = GetMonitorPosition(monitor_idx);
    TraceLog(
        LOG_INFO,
        "Monitor %d: %s (%dx%d) @ (%d, %d)",
        monitor_idx,
        GetMonitorName(monitor_idx),
        GetMonitorWidth(monitor_idx),
        GetMonitorHeight(monitor_idx),
        (int)monitor_position.x,
        (int)monitor_position.y
    );
  }

  // https://www.glfw.org/docs/3.3/intro_guide.html#coordinate_systems (glfw is default raylib backend)
  // since wayland has no such concept as a primary monitor, and doesnt allow clients to place themselves at specific
  // coordinates, i dont see any non-hacky way to find the top left corner of all monitors. raylib programs (as of now)
  // run via xwayland so we have window positioning, introducing a flag to have the user specify the leftmost monitor
  // should be enough to use SetWindowPosition() to place the window correctly until wayland itself figures this out.
  // for now we display the window at GetMonitorPosition(1) if there are multiple monitors. at least for my setup,
  // this displayed the window at the top left corner of the leftmost monitor as wanted.
  // TODO: introduce flag
  int     monitor_idx      = GetMonitorCount() == 1 ? 0 : 1;
  Vector2 monitor_position = GetMonitorPosition(monitor_idx);
  SetWindowPosition((int)monitor_position.x, (int)monitor_position.y);
}
