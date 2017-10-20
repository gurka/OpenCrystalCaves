#!/bin/bash
set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REL_DIR="$DIR/../release"
DEB_DIR="$DIR/../debug"
ECL_DIR="$DIR/../eclipse"

case $1 in
  'all')
    mkdir -p "$REL_DIR" && cd "$REL_DIR" && cmake ../occ -DCMAKE_BUILD_TYPE=release
    cd ..
    mkdir -p "$DEB_DIR" && cd "$DEB_DIR" && cmake ../occ -DCMAKE_BUILD_TYPE=debug
    cd ..
    mkdir -p "$ECL_DIR" && cd "$ECL_DIR" && cmake ../occ -DCMAKE_BUILD_TYPE=debug -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_VERSION=4.7
    ;;

  'release')
    mkdir -p "$REL_DIR" && cd "$REL_DIR" && cmake ../occ -DCMAKE_BUILD_TYPE=release
    ;;

  'debug')
    mkdir -p "$DEB_DIR" && cd "$DEB_DIR" && cmake ../occ -DCMAKE_BUILD_TYPE=debug
    ;;

  'eclipse')
    mkdir -p "$ECL_DIR" && cd "$ECL_DIR" && cmake ../occ -DCMAKE_BUILD_TYPE=debug -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_VERSION=4.7
    ;;

  *)
    echo "Usage: $0 [all | release | debug | eclipse]"
    ;;
esac
