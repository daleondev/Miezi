#!/bin/bash
cmake -S . -B build -GNinja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_FLAGS="-stdlib=libc++"
cmake --build build -j12
