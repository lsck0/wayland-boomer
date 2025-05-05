#pragma once

#include "./state.h"

#include <raylib.h>

#ifndef __DEFAULTS_H__
#define __DEFAULTS_H__

extern const char* WINDOW_TITLE;
extern const int   WINDOW_WIDTH; // used when image was loaded from file
extern const int   WINDOW_HEIGHT;
extern const float ZOOM_MIN;
extern const float ZOOM_MAX;
extern const float ZOOM_STEP;
extern const Color BACKGROUND_COLOR;
extern const State initial_wb_state;

#endif // __DEFAULTS_H__
