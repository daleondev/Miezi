#!/usr/bin/env bash

# === Usage function ===

usage() {
    cat <<EOF
Usage: $(basename "$0") [OPTIONS] <Build-Type>

Description:
  Build the project with the given Build-Type.

Options:
  -h, --help        Show this help message and exit
  -j [N]            Number of threads for building

Build-Types:
  Debug (Default)   Build with debug information
  Release           Build optimized for release

Examples:
  $(basename "$0") Debug
  $(basename "$0") Release
EOF
}

# === Print error function ===

COL_DEF="\033[0m"
COL_RED="\033[0;31m"

print_err() {
    echo -e "${COL_RED}$1${COL_DEF}\n"
}

# === Parse arguments ===

# defaults
BUILD_TYPE="Debug"
JOBS=`nproc`

# look for help flag -> print usage
for var in "$@"
do
    if [[ $var == "-h" || $var == "--help" ]]; then
        usage
        exit 0
    fi
done

# too many arguments -> error
if [[ $# -gt 3 ]]; then

    print_err "Error: Too many arguments"
    usage
    exit 1

# three arguments -> jobs flag and build-type
elif [[ $# -gt 2 ]]; then

    # jobs flag -> check argument
    if [[ $1 == "-j" || $1 == "--jobs" ]]; then

        re='^[0-9]+$'
        if ! [[ $2 =~ $re ]] ; then
            print_err "Error: Invalid argument: $2"
            usage
            exit 1
        fi

        JOBS=$2

    else

        print_err "Error: Invalid argument: $1"
        usage
        exit 1

    fi

    # build-type -> check argument
    if [[ $3 == "Debug" || $3 == "Release" ]]; then

        BUILD_TYPE=$3

    else

        print_err "Error: Invalid argument: $3"
        usage
        exit 1

    fi

# two arguments -> only jobs flag, no build-type
elif [[ $# -gt 1 ]]; then
    
    # jobs flag -> check argument
    if [[ $1 == "-j" || $1 == "--jobs" ]]; then

        re='^[0-9]+$'
        if ! [[ $2 =~ $re ]] ; then
            print_err "Error: Invalid argument: $2"
            usage
            exit 1
        fi

        JOBS=$2

    else

        print_err "Error: Invalid argument: $1"
        usage
        exit 1

    fi

# one argument -> only build-type
elif [[ $# -gt 0 ]]; then

    # build-type -> check argument
    if [[ $1 == "Debug" || $1 == "Release" ]]; then

        BUILD_TYPE=$1

    else

        print_err "Error: Invalid argument.\n"
        usage
        exit 1

    fi

fi

shopt -s expand_aliases
source ./scripts/setup.bash

cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CXX_FLAGS="-stdlib=libc++"
cmake --build build -j $JOBS
