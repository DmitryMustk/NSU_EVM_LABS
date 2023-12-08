#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <v*> to choose version"
    exit 1
fi

VERSION=$1

mkdir -p build && cd build && cmake -DVERSION=$VERSION .. && make
