# AxisDWM

**AxisDWM** is a highly customized [dwm](https://dwm.suckless.org/) build with a warm, pastel Gruvbox-inspired aesthetic, sensible keybindings, carefully selected patches, and a modern, functional status bar. Built for simplicity, performance, and daily usability on minimalist setups.

> ⚡ Powered by Arch Linux, configured by hand, styled for clarity.

## ✨ Features

* 🎨 **Warm custom color scheme** (Catppuccin/Gruvbox hybrid)
* 🧱 **Patched** dwm:

  * `status2d`, `vanitygaps`, `movestack`, `statuspadding`
  * `notitle`, `underline tags`
* ⛓ **Per-tag rules** for apps like Telegram, Firefox, Obsidian, etc.
* 💡 **Functional status bar** (via shell script):

  * Volume, brightness, keyboard layout
  * Temperature, CPU load, memory, uptime (off)
  * Package updates, Wi-Fi status, date & time
* 🧠 **Beautiful icon tags** (via Nerd Fonts) for semantic workspaces
* 🪟 Gaps, paddings, bar margins — fully adjustable via keybinds
* 🖱 XF86 support (volume, brightness, etc.)
* 🔒 Screen lock, screenshots, floating toggle, layout switching
* 🧹 Clean structure, with everything clearly commented in `config.h`

## 📸 Screenshot

## 🔧 Dependencies (Arch Linux)

Make sure you have these installed:

```bash
sudo pacman -S base-devel xorg xorg-xinit \
  libx11 libxft libxinerama xorg-xsetroot \
  alsa-utils lm_sensors libxkbfile brightnessctl \
  pamixer flameshot slock nerd-fonts
```

Also recommended:

* `yay` or another AUR helper
* `xinit`, `alacritty`, `firefox`, `lf` (used in config)
* [JetBrainsMono Nerd Font](https://github.com/ryanoasis/nerd-fonts) (used for bar & tags)

## 🚀 Installation

```bash
git clone https://github.com/df1gg/axis-dwm.git ~/.config/axis-dwm
cd ~/.config/axis-dwm
sudo make clean install
```

Then add to `.xinitrc`:

```bash
exec dwm
```

And start with:

```bash
startx
```

Or use it via a display manager of your choice.


## ⌨️ Keybindings (Super = Mod4)

| Keys                  | Action                        |
| --------------------- | ----------------------------- |
| `Super + Return`      | Launch terminal (`alacritty`) |
| `Super + p`           | Launch dmenu                  |
| `Super + b`           | Launch browser (Firefox)      |
| `Super + e`           | Launch `lf` file manager      |
| `Super + Shift + c`   | Close window                  |
| `Super + j/k`         | Focus next/prev window        |
| `Super + Shift + j/k` | Move window up/down           |
| `Super + Space`       | Toggle layout                 |
| `Super + f/m/t`       | Floating / Monocle / Tile     |
| `Super + [1–9]`       | Switch tags                   |
| `XF86Audio*`          | Volume control                |
| `XF86MonBrightness*`  | Brightness control            |
| `Super + Ctrl + L`    | Lock screen                   |

...and many more. Everything is listed and commented in `config.h`.

## 🎨 Tags

Icons are used to represent tag workspaces. Example:

| Tag | Icon | Purpose          |
| --- | ---- | ---------------- |
| 1   | ``  | Code / Neovim    |
| 2   | ``  | Terminal         |
| 3   | ``  | Browser          |
| 4   | ``  | System tools     |
| 5   | ``  | AI / LLM Studio  |
| 6   | ``  | Notes / Obsidian |
| 7   | ``  | Telegram / Chat  |
| 8   | ``  | Media / Music    |
| 9   | ``  | Sandbox / Junk   |

## 💡 Status Bar Modules

Each module uses `^c`/`^b` syntax for color. Current modules:

*   Pacman updates
*   Brightness (percentage)
*  /   Volume (or muted)
*   Keyboard layout (`us`, `ru`, etc.)
*   CPU temperature
* CPU load (1min)
* RAM usage
* IP address (IPv4, local)
*   Wi-Fi status
*   Clock

The bar script is located in `/scripts/status.sh`. You can extend it by adding more functions and appending them to the `xsetroot -name` line.

## 🧩 Included Patches

* `barpadding`
* `status2d`
* `vanitygaps`
* `movestack`
* `statuspadding`
* `notitle`
* `underline tags`

## 🧠 Philosophy

> “Minimal, but never bare. Beautiful, but not bloated.”

AxisDWM is designed to be the perfect balance between performance, comfort, and aesthetics. It embraces dwm's suckless roots while enhancing usability and modern productivity.

## 📜 License

This is a personal build based on [dwm](https://dwm.suckless.org/), which is MIT licensed. Feel free to fork, study, and adapt.

## 📫 Contact / Credit

Created and maintained by **[@df1gg](https://github.com/df1gg)**
Icons powered by [Nerd Fonts](https://www.nerdfonts.com/)
Theme inspired by Gruvbox Material
DWM base: [https://dwm.suckless.org/](https://dwm.suckless.org/)

---

> Feel free to fork and customize for your own rice! 🎉

