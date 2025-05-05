#include "./headers/controls.h"
#include "./headers/defaults.h"
#include "./headers/image.h"
#include "./headers/state.h"
#include "./headers/window.h"

#include <memory.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>

static State state;

int main(void) {
  SetTraceLogLevel(LOG_INFO);

  state = initial_wb_state;

  bool  was_file;
  Image img = load_image_from_stdin(&was_file, NULL);
  if (memcmp(&img, &(Image){0}, sizeof(Image)) == 0) return EXIT_FAILURE;

  init_raylib_window(img.width, img.height, was_file);

  Texture2D       img_texture        = LoadTextureFromImage(img);
  RenderTexture2D img_render_texture = LoadRenderTexture(img.width, img.height);
  UnloadImage(img);

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) break;
    handle_inputs(&state);

    BeginTextureMode(img_render_texture);
    ClearBackground(BACKGROUND_COLOR);
    DrawTextureEx(img_texture, state.pan, 0.0F, state.zoom, WHITE);
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
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
