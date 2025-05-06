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

// clang-format off
static const char* flashlight_frag_shader_source =
  "#version 330 core\n"
  "in vec2 fragTexCoord;\n"
  "out vec4 fragColor;\n"
  "uniform sampler2D texture0;\n"
  "uniform vec2 center;\n"
  "uniform float radius;\n"
  "void main(void)\n"
  "{\n"
  "    vec4 color = texture(texture0, fragTexCoord);\n"
  "    vec2 delta = gl_FragCoord.xy - center;\n"
  "    if (dot(delta, delta) > radius * radius) {\n"
  "        color.rgb *= 0.1;\n"
  "    }\n"
  "    fragColor = color;\n"
  "}\n";
// clang-format on

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

  Shader flashlight_shader = LoadShaderFromMemory(NULL, flashlight_frag_shader_source);
  int    loc_texture       = GetShaderLocation(flashlight_shader, "texture0");
  int    loc_center        = GetShaderLocation(flashlight_shader, "center");
  int    loc_radius        = GetShaderLocation(flashlight_shader, "radius");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) break;
    handle_inputs(&state);

    BeginTextureMode(img_render_texture);
    ClearBackground(BACKGROUND_COLOR);
    DrawTextureEx(img_texture, state.pan, 0.0F, state.zoom, WHITE);
    EndTextureMode();

    BeginDrawing();
    if (state.flashlight_enabled) {
      Vector2 m            = GetMousePosition();
      float   u_center[2]  = {m.x, (float)GetScreenHeight() - m.y};
      float   u_radius[1]  = {state.flashlight_radius};
      int     u_texture[1] = {0};
      SetShaderValue(flashlight_shader, loc_center, u_center, SHADER_UNIFORM_VEC2);
      SetShaderValue(flashlight_shader, loc_radius, u_radius, SHADER_UNIFORM_FLOAT);
      SetShaderValue(flashlight_shader, loc_texture, u_texture, SHADER_UNIFORM_INT);

      BeginShaderMode(flashlight_shader);
    }

    ClearBackground(BACKGROUND_COLOR);
    DrawTextureRec(
        img_render_texture.texture,
        (Rectangle){0, 0, (float)img_render_texture.texture.width, (float)-img_render_texture.texture.height},
        (Vector2){0, 0},
        WHITE
    );

    if (state.flashlight_enabled) {
      EndShaderMode();
    }

    EndDrawing();
  }

  UnloadShader(flashlight_shader);
  UnloadRenderTexture(img_render_texture);
  UnloadTexture(img_texture);
  CloseWindow();
  return 0;
}
