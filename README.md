<div align="center">
  <h1>wayland-boomer</h1>
  <h3>zoomer application for linux</h3>
</div>

WARNING: There currently seem to be raylib bugs which prevent multi-monitor panning/zooming/flashlight from working correctly.

## Usage

Fullscreen mode:

```sh
grim - | wayland-boomer

```

Image mode:

```sh
wayland-boomer < image.png

```

Currently supported image formats: png, jpg, webp, bmp.

## Keybindings

| Input                  | Action                   |
| ---------------------- | ------------------------ |
| `ESC` or `q`           | Quit                     |
| `0`                    | Reset                    |
| Left Mouse Button Drag | Pan                      |
| Mouse Wheel            | Zoom In/Out              |
| `f`                    | Toggle Flashlight        |
| `CTRL` + Mouse Wheel   | Change Flashlight Radius |
| `s`                    | Take a Screenshot        |

## Dependencies

- raylib

## Development

```sh
make
```

## References

- https://github.com/tsoding/boomer
