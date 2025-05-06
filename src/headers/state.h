#pragma once

#include <raylib.h>

#ifndef __STATE_H__
#define __STATE_H__

typedef struct {
  Vector2 pan;
  float   zoom;
  bool    flashlight_enabled;
  float   flashlight_radius;
} State;

#endif // __STATE_H__
