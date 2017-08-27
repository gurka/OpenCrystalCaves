#!/bin/bash
case $1 in
  'all')
    mkdir -p release && cd release && cmake ../occ -DCMAKE_BUILD_TYPE=release
    cd ..
    mkdir -p debug && cd debug && cmake ../occ -DCMAKE_BUILD_TYPE=debug
    cd ..
    mkdir -p eclipse && cd eclipse && cmake ../occ -DCMAKE_BUILD_TYPE=debug -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_VERSION=4.7
    ;;

  'release')
    mkdir -p release && cd release && cmake ../occ -DCMAKE_BUILD_TYPE=release
    ;;

  'debug')
    mkdir -p debug && cd debug && cmake ../occ -DCMAKE_BUILD_TYPE=debug
    ;;

  'eclipse')
    mkdir -p eclipse && cd eclipse && cmake ../occ -DCMAKE_BUILD_TYPE=debug -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_VERSION=4.7
    ;;

  *)
    echo "Usage: $0 [all | release | debug | eclipse]"
    ;;
esac
