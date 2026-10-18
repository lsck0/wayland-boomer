#include "boomer.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <rlgl.h>

/*
 * The vendored raylib forces GLFW_SCALE_FRAMEBUFFER off before creating the window, also with FLAG_WINDOW_HIGHDPI
 * (that flag only sets GLFW_SCALE_TO_MONITOR, which Wayland ignores). On a scaled Wayland output the framebuffer
 * then stays at the logical window size and the compositor upscales it, so the screenshot is resampled twice.
 * The linker routes raylib's glfwWindowHint() through here (-Wl,--wrap=glfwWindowHint) to keep it on: GLFW then
 * asks for the output's fractional scale and the framebuffer has native pixels. raylib still believes the render
 * size is the logical size, so everything drawn to the screen goes through framebuffer_begin() below.
 */
void __real_glfwWindowHint(int hint, int value);

__attribute__((used)) void __wrap_glfwWindowHint(int hint, int value) {
  if (hint == GLFW_SCALE_FRAMEBUFFER) value = GLFW_TRUE;
  __real_glfwWindowHint(hint, value);
}

Vector2 framebuffer_size(void) {
  int width  = 0;
  int height = 0;
  glfwGetFramebufferSize(GetWindowHandle(), &width, &height);
  return (Vector2){ (float)width, (float)height };
}

// framebuffer pixels per logical pixel: the output's scale once the compositor has sent it, 1 before
float framebuffer_scale(void) {
  int screen_width = GetScreenWidth();
  if (screen_width <= 0) return 1.0F;
  return framebuffer_size().x / (float)screen_width;
}

// raylib's EndTextureMode() resets the viewport to the logical size, call this after it
void framebuffer_begin(void) {
  Vector2 size = framebuffer_size();

  rlDrawRenderBatchActive();
  rlViewport(0, 0, (int)size.x, (int)size.y);
  rlMatrixMode(RL_PROJECTION);
  rlLoadIdentity();
  rlOrtho(0, size.x, size.y, 0, 0.0, 1.0);
  rlMatrixMode(RL_MODELVIEW);
  rlLoadIdentity();
}

// mouse position in framebuffer pixels, GLFW reports logical pixels on Wayland
Vector2 mouse_position(void) {
  return Vector2Scale(GetMousePosition(), framebuffer_scale());
}

Vector2 mouse_delta(void) {
  return Vector2Scale(GetMouseDelta(), framebuffer_scale());
}
