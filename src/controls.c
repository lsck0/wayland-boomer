#include "./headers/controls.h"

#define _POSIX_C_SOURCE 200809L // for popen()/pclose()

#include "./headers/draw.h"
#include "./headers/globals.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// from stb_image_write.h, which is part of raylib
unsigned char* stbi_write_png_to_mem(const unsigned char* pixels, int stride_bytes, int x, int y, int n, int* out_len);

static void handle_reset(void);
static void handle_panning(void);
static void handle_zoom(void);
static void handle_flashlight(void);
static void handle_screenshot(void);

void handle_inputs(void) {
  handle_reset();
  handle_panning();
  handle_zoom();
  handle_flashlight();
  handle_screenshot();
  handle_draw();
}

static void handle_reset(void) {
  if (IsKeyPressed(KEY_ZERO)) {
    *g_state = g_initial_state;
    lines_clear();
  }
}

static void handle_panning(void) {
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT && !g_state->is_drawing)) {
    Vector2 mouse_delta  = GetMouseDelta();
    g_state->pan.x      += mouse_delta.x;
    g_state->pan.y      += mouse_delta.y;
  }
}

static void handle_zoom(void) {
  float mouse_wheel_delta = GetMouseWheelMove();
  if (mouse_wheel_delta != 0 && !g_state->is_drawing && !IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_RIGHT_CONTROL)) {
    Vector2 mouse_pos = GetMousePosition();
    float   prev_zoom = g_state->zoom;
    Vector2 world     = {(mouse_pos.x - g_state->pan.x) / prev_zoom, (mouse_pos.y - g_state->pan.y) / prev_zoom};
    g_state->zoom     = Clamp(
        g_state->zoom + mouse_wheel_delta * g_configuration->zoom_step,
        g_configuration->zoom_min,
        g_configuration->zoom_max
    );
    g_state->pan.x = mouse_pos.x - world.x * g_state->zoom;
    g_state->pan.y = mouse_pos.y - world.y * g_state->zoom;
  }
}

static void handle_flashlight(void) {
  if (IsKeyPressed(KEY_F)) {
    g_state->flashlight_enabled = !g_state->flashlight_enabled;
  }

  float mouse_wheel_delta = GetMouseWheelMove();
  if (g_state->flashlight_enabled && mouse_wheel_delta != 0) {
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
      g_state->flashlight_radius -= mouse_wheel_delta * g_configuration->flashlight_radius_step;
      g_state->flashlight_radius  = Clamp(
          g_state->flashlight_radius,
          g_configuration->flashlight_radius_min,
          g_configuration->flashlight_radius_max
      );
    }
  }
}

// this is essentially raylib's TakeScreenshot() and ExportImage() decomposed
void handle_screenshot(void) {
  if (IsKeyDown(KEY_S)) {
    bool should_save_to_file = false;
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) should_save_to_file = true;

    int screen_width  = GetScreenWidth();
    int screen_height = GetScreenHeight();

    unsigned char* rlReadScreenPixels(int width, int height);
    unsigned char* raw_image_data = rlReadScreenPixels(screen_width, screen_height);
    Image          image          = {raw_image_data, screen_width, screen_height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    int            channels       = 4;

    unsigned long  fize_size       = 0;
    unsigned char* image_bytes_png = stbi_write_png_to_mem(
        raw_image_data,
        channels * screen_width,
        screen_width,
        screen_height,
        channels,
        (int*)&fize_size
    );

    if (image_bytes_png == NULL) {
      TraceLog(LOG_ERROR, "Failed to generate PNG image");
      goto cleanup;
    }

    if (should_save_to_file) {
      const char* folder;

      if (g_args->screenshot_folder != NULL) {
        folder = g_args->screenshot_folder;
      } else {
        folder = getenv("XDG_PICTURES_DIR");
      }

      if (folder == NULL) {
        folder = getenv("HOME");

        if (folder == NULL) {
          TraceLog(LOG_ERROR, "Could not find XDG_PICTURES_DIR or HOME environment variable");
          goto cleanup;
        }

        if (DirectoryExists(TextFormat("%s/Pictures", folder))) {
          folder = TextFormat("%s/Pictures", folder);
        } else if (DirectoryExists(TextFormat("%s/pictures", folder))) {
          folder = TextFormat("%s/pictures", folder);
        } else if (DirectoryExists(TextFormat("%s/Images", folder))) {
          folder = TextFormat("%s/Images", folder);
        } else if (DirectoryExists(TextFormat("%s/images", folder))) {
          folder = TextFormat("%s/images", folder);
        }
      }

      SetRandomSeed(time(NULL));
      int  nonce = GetRandomValue(1000000, 99999999);
      char file_name[1024];
      (void)sprintf(file_name, "%s/wayboomer_screenshot_%d.png", folder, nonce);

      bool result = SaveFileData(file_name, image_bytes_png, (int)fize_size);
      if (!result) {
        TraceLog(LOG_ERROR, "Failed to save screenshot to %s", file_name);
        goto cleanup;
      }

    } else {
      FILE* wl_copy_pipe = popen("wl-copy", "w");
      if (!wl_copy_pipe) {
        TraceLog(LOG_ERROR, "Failed to open pipe to wl-copy, is wl-copy installed?");
        (void)pclose(wl_copy_pipe);
        goto cleanup;
      }

      unsigned long bytes_written = fwrite(image_bytes_png, 1, fize_size, wl_copy_pipe);
      if (bytes_written != fize_size) {
        TraceLog(LOG_ERROR, "Failed to write screenshot to wl-copy");
        (void)pclose(wl_copy_pipe);
        goto cleanup;
      }

      (void)fflush(wl_copy_pipe);
      (void)pclose(wl_copy_pipe);
    }

  cleanup:
    RL_FREE(image_bytes_png);
    UnloadImage(image);
  }
}
