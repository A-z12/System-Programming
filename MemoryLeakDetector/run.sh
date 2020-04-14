#!/bin/bash

gcc -g -c mld.c -o mld.o ||  exit 1 

gcc -g -c app.c -o app.o || exit 2

gcc -g -o exe mld.o app.o ||  exit 3 

./exe
