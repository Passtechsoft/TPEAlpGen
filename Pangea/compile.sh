#!/bin/bash

cmake .
make
chmod +x PWGenExe

if [[ $1 == -l ]]
then
./PWGenExe
fi
