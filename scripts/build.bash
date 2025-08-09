#!/usr/bin/env bash
set -euo pipefail

# === Usage function ===

usage() {
    cat <<EOF
Usage: $(basename "$0") [OPTIONS] <Build-Type>

Description:
  Build the project with the given Build-Type.

Options:
    -h, --help          Show this help message and exit
    -j [N], --jobs [N]  Number of threads for building

Build-Types:
    Debug (Default)     Build with debug information
    Release             Build optimized for release

Examples:
  $(basename "$0") Debug
  $(basename "$0") Release
EOF
}

# === Print error function ===

COL_DEF="\033[0m"
COL_RED="\033[0;31m"

print_err() {
    echo -e "${COL_RED}$1${COL_DEF}\n" >&2
}

# === Parse arguments ===

# defaults
BUILD_TYPE="Debug"
JOBS=`nproc`
re_num='^[0-9]+$'

# check arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help)
            usage
            exit 0
            ;;
        -j|--jobs)
            shift
            if [[ $# -eq 0 || ! $1 =~ $re_num ]]; then
                print_err "Error: Missing or invalid number of jobs."
                usage
                exit 1
            fi
            JOBS="$1"
            ;;
        Debug|Release)
            BUILD_TYPE="$1"
            ;;
        *)
            print_err "Error: Invalid argument: $1"
            usage
            exit 1
            ;;
    esac
    shift
done

# === Build ===

shopt -s expand_aliases
source ./scripts/setup.bash

cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_CXX_FLAGS="-stdlib=libc++"
cmake --build build -j "$JOBS"
