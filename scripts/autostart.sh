#!/bin/bash

setxkbmap -layout "us,ru" -option "grp:alt_shift_toggle"

# запускается один раз через dwm.c
~/.config/axisdwm/scripts/status.sh &

picom &

feh --bg-fill ~/.config/wallpapers/wall.png &

flameshot &

