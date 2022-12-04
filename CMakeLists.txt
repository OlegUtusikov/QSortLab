cmake_minimum_required(VERSION 3.18)
project(QSortLab)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Ofast -march=native -mtune=native")

find_package(Threads REQUIRED)
find_package(Parlay REQUIRED)

add_executable(QSortLab main.cpp qsort_par.h qsort_seq_std.h qsort_seq_my.h utils.h)

target_link_libraries(QSortLab PUBLIC Parlay::parlay)
