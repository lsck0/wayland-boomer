#include "image.h"

#include <memory.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

int main(void) {
  SetTraceLogLevel(LOG_INFO);

  bool        was_file;
  const char* ext;
  Image       img = load_image_from_stdin(&was_file, &ext);
  if (memcmp(&img, &(Image){0}, sizeof(Image)) == 0) return EXIT_FAILURE;

  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
  InitWindow(1080, 720, "wayland-boomer");
  SetWindowFocused();

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

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) break;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }

  UnloadImage(img);
  CloseWindow();
  return 0;
}
