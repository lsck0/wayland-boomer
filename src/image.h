#pragma once

#include <raylib.h>

#ifndef __IMAGE_H__
#define __IMAGE_H__

Image load_image_from_stdin(bool* out_was_file, const char** out_ext);

#endif // __IMAGE_H__
