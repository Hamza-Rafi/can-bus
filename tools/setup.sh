#!/bin/bash
# Connect the attack STM32 (running slcan) as the CAN interface "attack".
# Run once after the Pi boots (needs root).

ATTACK_PORT=/dev/ttyACM0         # the only board plugged into the Pi

modprobe slcan

slcand -o -c -s6 -S 921600 "$ATTACK_PORT" attack
sleep 1
ip link set attack up

echo "Attack node ready:"
ip -brief link show attack
