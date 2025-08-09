#!/usr/bin/env bash
shopt -s expand_aliases
source ./scripts/setup.bash

cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-stdlib=libc++"
cmake --build build -j12
