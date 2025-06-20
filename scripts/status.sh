#!/bin/bash

# ^c$var^ = fg color
# ^b$var^ = bg color

interval=0

# load colors
. ~/.config/axisdwm/scripts/bar_themes/catppuccin

cpu() {
  cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)
  printf "^c$black^ ^b$green^ CPU"
  printf "^c$white^ ^b$grey^ $cpu_val"
}

pkg_updates() {
  updates=$({ timeout 20 checkupdates 2>/dev/null || true; } | wc -l)
  if [ -z "$updates" ] || [ "$updates" -eq 0 ]; then
    printf "  ^c$green^     Fully Updated"
  else
    printf "  ^c$green^     $updates updates"
  fi
}

battery() {
  # Найдём первый существующий путь батареи
  BAT_PATH=""
  for bat in /sys/class/power_supply/BAT*; do
    if [ -d "$bat" ]; then
      BAT_PATH="$bat"
      break
    fi
  done

  if [ -n "$BAT_PATH" ] && [ -f "$BAT_PATH/capacity" ]; then
    get_capacity=$(cat "$BAT_PATH/capacity")
    printf "^c$blue^   $get_capacity%%"
  else
    # Нет батареи
    printf "^c$blue^   N/A"
  fi
}

brightness() {
  printf "^c$red^   "
  printf "^c$red^%.0f\n" $(cat /sys/class/backlight/*/brightness)
}

mem() {
  printf "^c$blue^^b$black^  "
  printf "^c$blue^ $(free -h | awk '/^Mem/ { print $3 }' | sed s/i//g)"
}

wlan() {
  case "$(cat /sys/class/net/wl*/operstate 2>/dev/null)" in
  up) printf "^c$black^ ^b$blue^   ^d^%s" " ^c$blue^Connected" ;;
  down) printf "^c$black^ ^b$blue^ 󰤭  ^d^%s" " ^c$blue^Disconnected" ;;
  *) printf "^c$black^ ^b$blue^ 󰤭  ^d^%s" " ^c$blue^Disconnected" ;;
  esac
}

clock() {
  printf "^c$black^ ^b$darkblue^  "
  printf "^c$black^^b$blue^ $(date '+%Y-%m-%d %H:%M') "
}

while true; do
  [ $interval = 0 ] || [ $(($interval % 3600)) = 0 ] && updates=$(pkg_updates)
  interval=$((interval + 1))

  sleep 1 && xsetroot -name "$updates $(battery) $(brightness) $(cpu) $(mem) $(wlan) $(clock)"
done

