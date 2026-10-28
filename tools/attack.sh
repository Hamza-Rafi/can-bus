#!/bin/bash
# Play one attack log on the attack node.
# Usage: attack.sh /home/pi/dataset/reverse_light_off_attack_1.attackonly.log

if [ ! -f "$1" ]; then
    echo "No such attack log: $1"
    exit 1
fi

pkill -f "attack=can0"           # stop any attack that is still running

echo "Attack started: $(basename "$1")"
canplayer -I "$1" attack=can0
echo "Attack finished: $(basename "$1")"
