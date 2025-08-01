<div align="center">
  <h1>wayland-boomer</h1>
  <h3>zoomer application for linux</h3>
</div>

[![License](https://img.shields.io/github/license/lsck0/wayland-boomer)](LICENSE)
[![AUR Package](https://img.shields.io/aur/version/wayland-boomer-git)](https://aur.archlinux.org/packages/wayland-boomer-git)

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
  -sd <path>,     --screenshot-dir <path>                    Folder to save screenshots in.
  -bg <rgba hex>, --background <rgba hex>                    Background color.

```

All defaults can be changed, if there is a need for adding more options, please open an issue.

Because this is a native wayland window, positioning has to be done through the window manager.
In the case of Hyprland, this can be done with the following rules:

```
windowrule = float,     title:^wayland-boomer$
windowrule = monitor 1, title:^wayland-boomer$
windowrule = move 0 0,  title:^wayland-boomer$
windowrule = noanim,    title:^wayland-boomer$
```

When opening a file, the window title will be `wayland-boomer - image viewer`.

NOTE: You need to set `monitor` to your leftmost monitor.

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

## Installation

`wayland-boomer` is available on the AUR: [wayland-boomer-git](https://aur.archlinux.org/packages/wayland-boomer-git/).
You can install it using your AUR helper of choice, for example:

```sh
yay -S wayland-boomer-git
```

Alternatively, you can build it from source:

```sh
git clone https://github.com/lsck0/wayland-boomer.git
cd wayland-boomer
make build
```

## Dependencies

- glfw
- grim (for taking the screenshot, any other screenshot tool that can output to stdout works)
- wl-copy (optional, for screenshots to clipboard)

## Development

```sh
make
```

## References

- https://github.com/tsoding/boomer
