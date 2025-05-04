#include "raylib.h"

int main(void) {
  SetTraceLogLevel(LOG_INFO);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1080, 720, "wayland-boomer");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) break;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
