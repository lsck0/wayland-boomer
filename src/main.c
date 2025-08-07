#include "./headers/args.h"
#include "./headers/controls.h"
#include "./headers/draw.h"
#include "./headers/globals.h"
#include "./headers/image.h"

#include <math.h>
#include <memory.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>

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

int main(int argc, char** argv) {
  process_commandline_arguments(argc, argv);

  SetTraceLogLevel(LOG_INFO);

  bool  was_file;
  Image img = load_image_from_stdin(&was_file, NULL);
  if (memcmp(&img, &(Image){0}, sizeof(Image)) == 0) return EXIT_FAILURE;

  if (was_file) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(g_configuration->window_width, g_configuration->window_height, g_configuration->window_title_imagemode);
  } else {
    SetConfigFlags(
        FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_TRANSPARENT |
        FLAG_WINDOW_RESIZABLE
    );

    // compensate for monitor scaling: compositor multiplies the window size by the scaling factor
    int window_width  = (int)roundf((float)img.width / g_configuration->monitor_scaling);
    int window_height = (int)roundf((float)img.height / g_configuration->monitor_scaling);
    g_state->zoom     = 1 / g_configuration->monitor_scaling;

    InitWindow(window_width, window_height, g_configuration->window_title_boomermode);
  }

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
    handle_inputs();

    BeginTextureMode(img_render_texture);
    ClearBackground(g_configuration->background_color);
    DrawTextureEx(img_texture, g_state->pan, 0.0F, g_state->zoom, WHITE);
    lines_draw();
    EndTextureMode();

    BeginDrawing();
    if (g_state->flashlight_enabled) {
      Vector2 mouse_pos    = GetMousePosition();
      float   u_center[2]  = {mouse_pos.x, (float)GetScreenHeight() - mouse_pos.y};
      float   u_radius[1]  = {g_state->flashlight_radius};
      int     u_texture[1] = {0};
      SetShaderValue(flashlight_shader, loc_center, u_center, SHADER_UNIFORM_VEC2);
      SetShaderValue(flashlight_shader, loc_radius, u_radius, SHADER_UNIFORM_FLOAT);
      SetShaderValue(flashlight_shader, loc_texture, u_texture, SHADER_UNIFORM_INT);

      BeginShaderMode(flashlight_shader);
    }

    ClearBackground(g_configuration->background_color);
    DrawTextureRec(
        img_render_texture.texture,
        (Rectangle){0, 0, (float)img_render_texture.texture.width, (float)-img_render_texture.texture.height},
        (Vector2){0, 0},
        WHITE
    );

    if (g_state->flashlight_enabled) {
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
