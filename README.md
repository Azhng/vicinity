# Vicinity - Distributed Real-Time image processing pipeline

Design doc: https://docs.google.com/document/d/1QZ93JeKcHi5c9KIWQmusRuJG1kim8K3v0Ucr8kU5ZUc/edit?usp=sharing

## Phase 1 [x]:
Finalize pipeline API

## Phase 2 [WIP]:
Using type system to verify pipeline correctness at compile time

## Phase 3 [Spiking]:
Going distributed


## Build

This project was developed against C++17, OpenCV 4.0 and Boost 1.67.

``` bash
cd /path/to/viciniy
mkdir build/
cd build

# change the flags based on your needs
cmake -DRELEASE=OFF -DBUILD_TEST=ON -DBUILD_EXAMPLE=ON ..
make -j4

# to run test 
ctest
```
