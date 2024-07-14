#!/bin/bash

if [[ -d build ]]; then
    echo "Removing old build directory."
    echo
    rm -r build
fi

mkdir build

cd build

cmake .. -GNinja
