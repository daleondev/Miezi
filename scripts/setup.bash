#!/usr/bin/env bash

# ==== CONFIG ====
LLVM_LIB_PATH="$HOME/development/llvm-install/lib/x86_64-unknown-linux-gnu/"
CMAKE_PATH="$HOME/development/cmake-install/bin/cmake"
CLANG_PATH="$HOME/development/llvm-install/bin/clang++"
# ================

# Add to LD_LIBRARY_PATH only if not already present
case ":${LD_LIBRARY_PATH:-}:" in
    *":$LLVM_LIB_PATH:"*) ;;
    *)
        if [[ -n "${LD_LIBRARY_PATH:-}" ]]; then
            export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$LLVM_LIB_PATH"
        else
            export LD_LIBRARY_PATH="$LLVM_LIB_PATH"
        fi
        ;;
esac

# Export compiler for build tools
export CXX="$CLANG_PATH"

# Set aliases
alias cmake="$CMAKE_PATH"
alias clang++="$CLANG_PATH"

echo "Environment configured for this session:"
echo "  LD_LIBRARY_PATH = $LD_LIBRARY_PATH"
echo "  CXX = $CXX"
echo "  cmake -> $CMAKE_PATH (`cmake --version | awk '/version/{print $3F}'`)"
echo "  clang++ -> $CLANG_PATH (`clang++ --version | awk '/version/{print $3F}'`)"
echo ""