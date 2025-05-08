<div align="center">
  <h1>wayland-boomer</h1>
  <h3>zoomer application for linux</h3>
</div>

WARNING: There currently seem to be raylib bugs which prevent multi-monitor panning/zooming/flashlight from working correctly.

## Usage

```sh
Usage:
  grim - | ./wayland-boomer [options]                        Boomer Mode
  ./wayland-boomer [options] < image.[png|jpg|webp|bmp]      Image Viewer Mode
Options:
  -h,             --help                                     Show this message and exit.
  -v,             --version                                  Show version and exit.
  -lmm <int>,     --leftmost-monitor <int>                   Monitor to place the window origin on.
  -sd <path>,     --screenshot-dir <path>                    Folder to save screenshots in.
  -bg <rgba hex>, --background <rgba hex>                    Background color.

```

## Keybindings

| Input                   | Action                                                  |
| ----------------------- | ------------------------------------------------------- |
| `ESC` or `q`            | Quit                                                    |
| `0`                     | Reset                                                   |
| Left Mouse Button Drag  | Pan                                                     |
| Mouse Wheel             | Zoom In/Out                                             |
| `f`                     | Toggle Flashlight                                       |
| `CTRL` + Mouse Wheel    | Change Flashlight Radius                                |
| `s`                     | Take a Screenshot to Clipboard (needs wl-copy)          |
| `CTRL` + `s`            | Take a Screenshot to File (to $XDG_PICTURE_DIR / $HOME) |
| Right Mouse Button Drag | Draw                                                    |

## Dependencies

- raylib
- wl-copy (optional, for screenshots to clipboard)

## Development

```sh
make
```

## References

- https://github.com/tsoding/boomer
