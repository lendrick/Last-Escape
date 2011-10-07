#!/bin/sh

#if build directory doesn't exists
if [ ! -d "build" ]; then
    mkdir build
    echo "created build directory"
fi

#sucessed to create build directory
if [ -d "build" ]; then   
    cd build
    cmake ../src
    make
else
    echo "Failed to create build directory"
fi
