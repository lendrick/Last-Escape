#!/bin/sh
cd build || mkdir build && cd build
cmake ../src
make
