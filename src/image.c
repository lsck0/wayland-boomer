#include "./headers/image.h"

#include <memory.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static const char* detect_image_extension(const unsigned char* data, size_t length);

Image load_image_from_stdin(bool* out_was_file, const char** out_ext) {
  struct stat    st       = {0};
  unsigned char* buffer   = NULL;
  size_t         length   = 0;
  size_t         capacity = 0;
  bool           is_file  = false;
  const char*    ext      = NULL;

  TraceLog(LOG_WARNING, "If this is the last message you see, you're probably doing something wrong.");
  TraceLog(LOG_WARNING, "You are meant to provide an image via stdin.");
  TraceLog(LOG_WARNING, "Do: grim - | wayland-boomer");
  TraceLog(LOG_WARNING, "Or: wayland-boomer < image.png");

  if (fstat(STDIN_FILENO, &st) == 0 && S_ISREG(st.st_mode)) {
    // stdin is a regular file: allocate the file size
    is_file = true;

    capacity = st.st_size;
    buffer   = malloc(capacity);
    if (!buffer) {
      TraceLog(LOG_ERROR, "Could not allocate %zu bytes", capacity);
      goto on_error;
    }

    length = fread(buffer, 1, capacity, stdin);
    if (length != capacity && ferror(stdin)) {
      TraceLog(LOG_ERROR, "Could not read %zu bytes from stdin", capacity);
      goto on_error;
    }
  } else {
    // stdin is a pipe/tty/etc: read in chunks and grow the buffer
    char    tmp[BUFSIZ];
    ssize_t n;

    while ((n = read(STDIN_FILENO, tmp, sizeof(tmp))) > 0) {
      // grow buffer if needed
      if (length + (size_t)n > capacity) {
        size_t new_capacity = capacity ? capacity * 2 : BUFSIZ;
        if (new_capacity < length + (size_t)n) new_capacity = length + (size_t)n;

        unsigned char* p = realloc(buffer, new_capacity);
        if (!p) {
          TraceLog(LOG_ERROR, "Could not allocate %zu bytes", new_capacity);
          goto on_error;
        }

        buffer   = p;
        capacity = new_capacity;
      }

      memcpy(buffer + length, tmp, n);
      length += (size_t)n;
    }

    if (n < 0) {
      TraceLog(LOG_ERROR, "Could not read from stdin");
      goto on_error;
    }
  }

  ext = detect_image_extension(buffer, length);
  if (!ext) {
    TraceLog(
        LOG_ERROR,
        "Could not detect image. Either you didn't provide a valid image or the image format is not supported."
    );
    goto on_error;
  }

  Image img = LoadImageFromMemory(ext, buffer, (int)length);
  if (img.data == NULL) {
    TraceLog(LOG_ERROR, "Could not load image from memory");
    goto on_error;
  }

  free(buffer);

  if (out_was_file) *out_was_file = is_file;
  if (out_ext) *out_ext = ext;
  return img;

on_error:
  free(buffer);
  if (out_was_file) *out_was_file = is_file;
  if (out_ext) *out_ext = ext;
  return (Image){0};
}

static const char* detect_image_extension(const unsigned char* data, size_t length) {
  if (length >= 8 && !memcmp(data, "\211PNG\r\n\032\n", 8)) return ".png";
  if (length >= 2 && !memcmp(data, "\xFF\xD8", 2)) return ".jpg";
  if (length >= 4 && !memcmp(data, "WEBP", 4)) return ".webp";
  if (length >= 2 && !memcmp(data, "BM", 2)) return ".bmp";

  return NULL;
}
