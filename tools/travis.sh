#!/bin/bash
set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DEBUG_DIR="$DIR/../debug"

mkdir -p "$DEBUG_DIR"
cd "$DEBUG_DIR"
cmake ../occ -DCMAKE_BUILD_TYPE=debug
cmake --build .
ctest -V
