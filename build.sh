#!/bin/bash
current_folder=$(basename "$PWD")
if [ "$current_folder" = "3_DataStructures1" ]; then
    rm -fdr build
    mkdir build
    cd build 
    cmake ..
    make
else
    cd ./3_DataStructures1
    rm -fdr build
    mkdir build
    cd build
    cmake ..
    make
fi
