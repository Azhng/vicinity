cmake_minimum_required(VERSION 2.8)

project( vicinity )

find_package( OpenCV REQUIRED )
find_package( Boost COMPONENTS system REQUIRED )

enable_testing()

include_directories( ${Boost_INCLUDE_DIRS} )

set (CMAKE_CXX_FLAGS "--std=c++17 -I/usr/local/include/opencv2 ${CMAKE_CXX_FLAGS}")

option(RELEASE "Build with optimize flag" OFF)
option(BUILD_TEST "Building unit tests" OFF)
option(BUILD_EXAMPLE "Building examples" OFF)
message(STATUS "Building Release version: ${RELEASE}")
message(STATUS "Building unit tests: ${BUILD_TEST}")
message(STATUS "BOOST_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")

subdirs(src)

if(RELEASE)
	set (CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS}")
else()
	set (CMAKE_CXX_FLAGS "-MMD -Wall -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -g ${CMAKE_CXX_FLAGS}")
endif()

if(BUILD_TEST) 
	message(STATUS "Building Tests")
	subdirs(test)
endif()

if(BUILD_EXAMPLE)
    message(STATUS "Building Examples")
    subdirs(example)
endif()
