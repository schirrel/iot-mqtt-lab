#!/bin/sh

echo "Compiling each one"

gcc -c constants.c                     
gcc -c mqtt_pub.c                   

echo "Generate mqtt_pub exec"
gcc constants.o mqtt_pub.o -o mqtt_pub -lmosquitto

echo "Starting mqtt_pub"
./mqtt_pub