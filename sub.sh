#!/bin/sh

echo "Compiling each one"

gcc -c constants.c                     
gcc -c mqtt_sub.c                   

echo "Generate mqtt_sub exec"
gcc constants.o mqtt_sub.o -o mqtt_sub -lmosquitto

echo "Starting mqtt_sub"
./mqtt_sub