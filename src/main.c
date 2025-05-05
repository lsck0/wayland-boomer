#include "image.h"
#include "window.h"

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

  init_raylib_window(img.width, img.height, was_file);

  Texture2D       img_texture        = LoadTextureFromImage(img);
  RenderTexture2D img_render_texture = LoadRenderTexture(2 * img.width, 2 * img.height);
  UnloadImage(img);

  Vector2 panning = {0, 0};

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) break;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      // BUG: raylib's GetMousePosition() is broken when the window is on multiple monitors
      Vector2 d  = GetMouseDelta();
      panning.x += d.x;
      panning.y += d.y;

      // Vector2 mouse_pos = GetMousePosition();
      // TraceLog(LOG_INFO, "Mouse position: %f %f", mouse_pos.x, mouse_pos.y);
    }

    BeginTextureMode(img_render_texture);
    ClearBackground(BLACK);
    DrawTextureEx(img_texture, panning, 0.0F, 1.0F, WHITE);
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextureRec(
        img_render_texture.texture,
        (Rectangle){0, 0, (float)img_render_texture.texture.width, (float)-img_render_texture.texture.height},
        (Vector2){0, 0},
        WHITE
    );
    EndDrawing();
  }

  UnloadRenderTexture(img_render_texture);
  UnloadTexture(img_texture);
  CloseWindow();
  return 0;
}
