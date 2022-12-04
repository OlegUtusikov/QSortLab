#!/bin/bash

export PARLAY_NUM_THREADS=4

rm -rf build_QSortLab
mkdir build_QSortLab

cd build_QSortLab

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake build .
make -j4 all

./QSortLab