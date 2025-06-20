#!/bin/bash

# ^c$var^ = fg color
# ^b$var^ = bg color

interval=0

# load colors
. ~/.config/axisdwm/scripts/bar_themes/catppuccin


pkg_updates() {
  updates=$({ timeout 20 checkupdates 2>/dev/null || true; } | wc -l)
  if [ -z "$updates" ] || [ "$updates" -eq 0 ]; then
    printf "  ^c$green^     Fully Updated"
  else
    printf "  ^c$green^     $updates updates"
  fi
}

#battery() {
#  BAT_PATH=""
#  for bat in /sys/class/power_supply/BAT*; do
#    if [ -d "$bat" ]; then
#      BAT_PATH="$bat"
#      break
#    fi
#  done
#
#  if [ -n "$BAT_PATH" ] && [ -f "$BAT_PATH/capacity" ]; then
#    get_capacity=$(cat "$BAT_PATH/capacity")
#    printf "^c$blue^   $get_capacity%%"
#  else
#    # Нет батареи
#    printf "^c$blue^   N/A"
#  fi
#}

brightness() {
    max_brightness=$(cat /sys/class/backlight/*/max_brightness)
    current_brightness=$(cat /sys/class/backlight/*/brightness)
    percent=$((100 * current_brightness / max_brightness))
    printf "^c$red^  "
    printf "^c$red^ %s%%" "$percent"
}

volume() {
    vol="N/A"
    muted="no"

    if command -v pactl >/dev/null 2>&1; then
        vol=$(pactl get-sink-volume @DEFAULT_SINK@ 2>/dev/null | grep -o '[0-9]\+%' | head -1)
        muted=$(pactl get-sink-mute @DEFAULT_SINK@ 2>/dev/null | awk '{print $2}')
    elif command -v amixer >/dev/null 2>&1; then
        vol=$(amixer get Master 2>/dev/null | grep -o '[0-9]\+%' | head -1)
        amixer get Master 2>/dev/null | grep -q '\[off\]' && muted="yes"
    fi

    if [ -z "$vol" ]; then
        vol="N/A"
    fi

    icon=""
    out="$vol"

    if [ "$muted" = "yes" ] || [ "$muted" = "off" ]; then
        icon=""
        out="muted"
    fi

    printf "^c$blue^ $icon "
    printf "^c$blue^ %s" "$out"
}

keyboard() {
    layout=$(xkb-switch -p 2>/dev/null)

    if [ -z "$layout" ]; then
        layout=$(setxkbmap -query | awk '/layout:/ {print $2}' | cut -d',' -f1)
    fi

    printf "^c$yellow^  "
    printf "^c$yellow^ %s" "$layout"
}

temperature() {
  temp=$(sensors | awk '/^Package id 0:/ {print $4}' | tr -d '+°C')
  [ -z "$temp" ] && temp="N/A"
  printf "^c$black^ ^b$red^   "
  printf "^c$white^ ^b$grey^ %s°C" "$temp"
}

cpu() {
  cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)
  printf "^c$black^ ^b$green^ CPU"
  printf "^c$white^ ^b$grey^ $cpu_val"
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

ip_address() {
    ip=$(ip a | awk '/inet / && !/127.0.0.1/ && !/inet6/ {print $2}' | cut -d/ -f1 | head -n1)
    [ -z "$ip" ] && ip="N/A"
    printf "^d^^c$green^  "
    printf "^c$green^ %s" "$ip"
}

uptime_info() {
    up=$(uptime -p | sed 's/up //')
    printf "^c$blue^  "
    printf "^c$blue^ %s" "$up"
}

clock() {
  printf "^c$black^ ^b$darkblue^  "
  printf "^c$black^^b$blue^ $(date '+%Y-%m-%d %H:%M') "
}


while true; do
  [ $interval = 0 ] || [ $(($interval % 3600)) = 0 ] && updates=$(pkg_updates)
  interval=$((interval + 1))

  sleep 1 && xsetroot -name "$updates $(brightness) $(volume) $(keyboard) $(temperature) $(cpu) $(mem) $(ip_address) $(wlan) $(clock)"
done

