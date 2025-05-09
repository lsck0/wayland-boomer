<div align="center">
  <h1>wayland-boomer</h1>
  <h3>zoomer application for linux</h3>
</div>

DISCLAIMER: Many features on a multi-monitor setup are currently not working correctly, see [#4931](https://github.com/raysan5/raylib/issues/4931).

## Demo

![Demo](https://raw.githubusercontent.com/lsck0/wayland-boomer/master/assets/demo.gif)

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

All defaults can be changed, if there is a need for adding more options, please open an issue.

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

- grim (for taking the screenshot)
- raylib (the graphics library)
- wl-copy (optional, for screenshots to clipboard)

## Development

```sh
make
```

## References

- https://github.com/tsoding/boomer
