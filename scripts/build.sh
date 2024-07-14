#!/bin/bash
if [[ ! -d build ]]; then
    ./scripts/init_build_dir.sh
    echo
fi

cmake --build build
