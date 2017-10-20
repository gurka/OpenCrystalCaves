#!/bin/bash
set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

mkdir -p "$DIR/../debug"
cd "$DIR/../debug"
cmake ../occ
cmake --build . -- -j2
