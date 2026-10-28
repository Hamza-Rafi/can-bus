#!/bin/bash
# Play the ambient log on the ambient STM32 (running slcan), looping forever.
# Usage: sudo ./ambient.sh /path/to/ambient.log [serial port]
# Stop with Ctrl+C.

AMBIENT_LOG=$1
PORT=${2:-/dev/ttyACM0}          # the ambient board, /dev/ttyACM0 unless given

if [ ! -f "$AMBIENT_LOG" ]; then
    echo "usage: sudo $0 <ambient log> [serial port]"
    exit 1
fi

# connect the board as the CAN interface "ambient"
modprobe slcan
slcand -o -c -s6 -S 921600 "$PORT" ambient
sleep 1
ip link set ambient up

# when this script stops (Ctrl+C), disconnect the board again
trap 'ip link set ambient down; pkill -f "slcand.*ambient"; echo "Ambient stopped"' EXIT

echo "Playing $(basename "$AMBIENT_LOG") on the bus, Ctrl+C to stop"
canplayer -l i -I "$AMBIENT_LOG" ambient=can0
