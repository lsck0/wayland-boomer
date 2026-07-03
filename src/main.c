#include "boomer.h"

// clang-format off
static const char* flashlight_frag_shader_source =
  "#version 330 core\n"
  "in vec2 fragTexCoord;\n"
  "out vec4 fragColor;\n"
  "uniform sampler2D texture0;\n"
  "uniform vec2 center;\n"
  "uniform float radius;\n"
  "uniform float darkness;\n"
  "void main(void)\n"
  "{\n"
  "    vec4 color = texture(texture0, fragTexCoord);\n"
  "    vec2 delta = gl_FragCoord.xy - center;\n"
  "    if (dot(delta, delta) > radius * radius) {\n"
  "        color.rgb *= darkness;\n"
  "    }\n"
  "    fragColor = color;\n"
  "}\n";
// clang-format on

int main(int argc, char** argv) {
  process_commandline_arguments(argc, argv);

  SetTraceLogLevel(LOG_INFO);

  bool  was_file;
  Image img = load_image_from_stdin(&was_file);
  if (memcmp(&img, &(Image){ 0 }, sizeof(Image)) == 0) return EXIT_FAILURE;

  if (was_file) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(g_configuration->window_width, g_configuration->window_height, g_configuration->window_title_imagemode);
  } else {
    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_RESIZABLE);

    // compensate for monitor scaling: compositor multiplies the window size by the scaling factor
    int window_width             = (int)roundf((float)img.width / g_configuration->monitor_scaling);
    int window_height            = (int)roundf((float)img.height / g_configuration->monitor_scaling);
    g_state->zoom_current        = 1 / g_configuration->monitor_scaling;
    g_state->zoom_target         = 1 / g_configuration->monitor_scaling;
    g_initial_state.zoom_current = g_state->zoom_current;
    g_initial_state.zoom_target  = g_state->zoom_target;

    InitWindow(window_width, window_height, g_configuration->window_title_boomermode);
  }

  Texture2D img_texture = LoadTextureFromImage(img);
  SetTextureFilter(img_texture, TEXTURE_FILTER_POINT);
  RenderTexture2D img_render_texture = LoadRenderTexture(img.width, img.height);
  SetTextureFilter(img_render_texture.texture, TEXTURE_FILTER_POINT);
  UnloadImage(img);

  Shader flashlight_shader = LoadShaderFromMemory(NULL, flashlight_frag_shader_source);
  int    loc_texture       = GetShaderLocation(flashlight_shader, "texture0");
  int    loc_center        = GetShaderLocation(flashlight_shader, "center");
  int    loc_radius        = GetShaderLocation(flashlight_shader, "radius");
  int    loc_darkness      = GetShaderLocation(flashlight_shader, "darkness");

  SetTargetFPS(120);
  while (!WindowShouldClose()) {

    /*
     * ─────────────────────────────────────────────────────────
     * INPUTS
     * ─────────────────────────────────────────────────────────
     */

    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) break;
    handle_inputs();

    /*
     * ─────────────────────────────────────────────────────────
     * UPDATE
     * ─────────────────────────────────────────────────────────
     */

    float dt       = GetFrameTime();
    float z_smooth = 1.0F - expf(-g_configuration->pan_rigidity * dt);
    float r_smooth = 1.0F - expf(-g_configuration->flashlight_radius_rigidity * dt);
    float d_smooth = 1.0F - expf(-g_configuration->flashlight_darkness_rigidity * dt);

    g_state->zoom_current = Lerp(g_state->zoom_current, g_state->zoom_target, z_smooth);
    g_state->pan_current  = Vector2Lerp(g_state->pan_current, g_state->pan_target, z_smooth);

    // ENABLE
    if (!g_state->flashlight_enabled_previously && g_state->flashlight_enabled) {
      g_state->flashlight_radius_current   = 1000.0;
      g_state->flashlight_darkness_current = 1.0;
      g_state->flashlight_rendering        = true;
    }
    g_state->flashlight_enabled_previously = g_state->flashlight_enabled;

    // ANIMATION
    if (g_state->flashlight_rendering) {
      if (g_state->flashlight_enabled) {
        g_state->flashlight_radius_current   = Lerp(g_state->flashlight_radius_current, g_state->flashlight_radius_target, r_smooth);
        g_state->flashlight_darkness_current = Lerp(g_state->flashlight_darkness_current, g_configuration->flashlight_darkness, d_smooth);
      } else {
        g_state->flashlight_radius_current   = Lerp(g_state->flashlight_radius_current, 1000.0F, r_smooth);
        g_state->flashlight_darkness_current = Lerp(g_state->flashlight_darkness_current, 1.0F, d_smooth);

        if (fabsf(g_state->flashlight_radius_current - 1000.0F) < 5.0F && fabsf(g_state->flashlight_darkness_current - 1.0F) < 0.05F) {
          g_state->flashlight_rendering        = false;
          g_state->flashlight_radius_current   = g_configuration->flashlight_radius_initial;
          g_state->flashlight_darkness_current = g_configuration->flashlight_darkness;
        }
      }
    }

    /*
     * ─────────────────────────────────────────────────────────
     * RENDER
     * ─────────────────────────────────────────────────────────
     */

    BeginTextureMode(img_render_texture);
    ClearBackground(g_configuration->background_color);
    DrawTextureEx(img_texture, g_state->pan_current, 0.0F, g_state->zoom_current, WHITE);
    lines_draw();
    EndTextureMode();

    BeginDrawing();
    if (g_state->flashlight_rendering) {
      Vector2 mouse_pos     = GetMousePosition();
      int     u_texture[1]  = { 0 };
      float   u_center[2]   = { mouse_pos.x, (float)GetScreenHeight() - mouse_pos.y };
      float   u_radius[1]   = { g_state->flashlight_radius_current };
      float   u_darkness[1] = { g_state->flashlight_darkness_current };
      SetShaderValue(flashlight_shader, loc_texture, u_texture, SHADER_UNIFORM_INT);
      SetShaderValue(flashlight_shader, loc_center, u_center, SHADER_UNIFORM_VEC2);
      SetShaderValue(flashlight_shader, loc_radius, u_radius, SHADER_UNIFORM_FLOAT);
      SetShaderValue(flashlight_shader, loc_darkness, u_darkness, SHADER_UNIFORM_FLOAT);

      BeginShaderMode(flashlight_shader);
    }

    ClearBackground(g_configuration->background_color);
    float     tex_width     = (float)img_render_texture.texture.width;
    float     tex_height    = (float)img_render_texture.texture.height;
    Rectangle mirror_source = { 0, 0, g_state->is_mirrored ? -tex_width : tex_width, -tex_height };
    float     mirror_dest_x = g_state->is_mirrored ? (float)GetScreenWidth() - tex_width : 0.0F;
    DrawTexturePro(
        img_render_texture.texture,
        mirror_source,
        (Rectangle){
            mirror_dest_x,
            0,
            tex_width,
            tex_height,
        },
        (Vector2){ 0, 0 },
        0.0F,
        WHITE
    );

    if (g_state->flashlight_rendering) EndShaderMode();

    EndDrawing();
  }

  UnloadShader(flashlight_shader);
  UnloadRenderTexture(img_render_texture);
  UnloadTexture(img_texture);
  CloseWindow();
  return 0;
}
