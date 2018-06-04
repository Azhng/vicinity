# Vicinity - Distributed Real-Time image processing pipeline

Currently WIP

## Phase 1:
Finalize pipeline API

## Phase 2:
Using type system to verify pipeline correctness at compile time

## Phase 3:
Going distributed


## Build

This project was developed against C++17, OpenCV 4.0 and Boost 1.67.

``` bash
cd /path/to/viciniy
mkdir build/
cd build


cmake -DRELEASE=OFF -DBUILD_TEST=ON ..
make -j4

# to run test 
ctest
```
