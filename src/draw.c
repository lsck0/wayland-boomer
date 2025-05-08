#include "./headers/globals.h"

#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  Vector2* points;
  float    thickness;
  int      points_count;
  int      points_capacity;
} Line;

static Line* lines          = NULL;
static int   lines_count    = 0;
static int   lines_capacity = 0;

static Vector2 to_texture_coords(Vector2 screen_pos);
static Vector2 to_screen_coords(Vector2 texture_pos);
static void    line_begin(void);
static void    line_add_point(Vector2 texture_pos);

void handle_draw(void) {
  bool right_down = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);

  if (right_down) {
    if (!g_state->is_drawing) {
      line_begin();
      g_state->is_drawing = true;
    }

    line_add_point(to_texture_coords(GetMousePosition()));
  } else {
    g_state->is_drawing = false;
  }
}

void lines_draw(void) {
  for (int i = 0; i < lines_count; i++) {
    Line* line = &lines[i];

    int n = line->points_count;
    if (n == 0) return;

    float screen_radius    = line->thickness * g_state->zoom;
    float screen_thickness = screen_radius * 2.0F;

    if (n == 1) {
      Vector2 p = to_screen_coords(line->points[0]);
      DrawCircleV(p, screen_radius, g_configuration->draw_color);
      return;
    }

    if (n == 2) {
      Vector2 p0 = to_screen_coords(line->points[0]);
      Vector2 p1 = to_screen_coords(line->points[1]);
      DrawLineEx(p0, p1, screen_thickness, g_configuration->draw_color);
      DrawCircleV(p1, screen_radius, g_configuration->draw_color);
      return;
    }

    // start
    Vector2 start = to_screen_coords(line->points[0]);
    DrawCircleV(start, screen_radius, g_configuration->draw_color);

    // segments
    for (int i = 0; i < n - 2; i++) {
      Vector2 P0 = to_screen_coords(line->points[i + 0]);
      Vector2 P1 = to_screen_coords(line->points[i + 1]);
      Vector2 P2 = to_screen_coords(line->points[i + 2]);

      Vector2 average_p0_p1 = Vector2Scale(Vector2Add(P0, P2), 0.5F);
      Vector2 ctrl          = Vector2Subtract(Vector2Scale(P1, 2.0F), average_p0_p1);

      DrawSplineSegmentBezierQuadratic(P0, ctrl, P2, screen_thickness, g_configuration->draw_color);
    }

    // end
    Vector2 end = to_screen_coords(line->points[n - 1]);
    DrawCircleV(end, screen_radius, g_configuration->draw_color);
  }
}

void lines_clear(void) {
  for (int i = 0; i < lines_count; i++) {
    free(lines[i].points);
  }
  free(lines);
  lines          = NULL;
  lines_count    = 0;
  lines_capacity = 0;
}

static void line_begin(void) {
  if (lines_count >= lines_capacity) {
    int   new_lines_capacity = (lines_capacity == 0) ? 4 : lines_capacity * 2;
    Line* new_lines          = realloc(lines, sizeof(Line) * new_lines_capacity);
    assert(new_lines);

    lines          = new_lines;
    lines_capacity = new_lines_capacity;
  }

  Line* new_line = &lines[lines_count++];
  *new_line      = (Line){
           .thickness = g_configuration->draw_thickness / g_state->zoom,
  };
}

static void line_add_point(Vector2 texture_pos) {
  Line* current_line = &lines[lines_count - 1];

  if (current_line->points_count >= current_line->points_capacity) {
    int      new_points_capacity = (current_line->points_capacity == 0) ? 64 : current_line->points_capacity * 2;
    Vector2* new_points          = realloc(current_line->points, sizeof(Vector2) * new_points_capacity);
    assert(new_points);

    current_line->points          = new_points;
    current_line->points_capacity = new_points_capacity;
  }

  current_line->points[current_line->points_count++] = texture_pos;
}

static Vector2 to_texture_coords(Vector2 screen_pos) {
  return Vector2Scale(Vector2Subtract(screen_pos, g_state->pan), 1.0F / g_state->zoom);
}

static Vector2 to_screen_coords(Vector2 texture_pos) {
  return Vector2Add(Vector2Scale(texture_pos, g_state->zoom), g_state->pan);
}
