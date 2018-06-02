cmake_minimum_required(VERSION 2.8)

project( vicinity )

find_package( OpenCV REQUIRED )
find_package( Boost COMPONENTS system REQUIRED )

enable_testing()

include_directories( ${BOOST_INCLUDE_DIR} )

set (CMAKE_CXX_FLAGS "--std=c++1z -I/usr/local/include/opencv2 ${CMAKE_CXX_FLAGS}")

option(RELEASE "Build with optimize flag" OFF)
option(BUILD_TEST "Building unit tests" OFF)
message(STATUS "Building Release version: ${RELEASE}")
message(STATUS "Building unit tests: ${BUILD_TEST}")

subdirs(src)

if(RELEASE)
	set (CMAKE_CXX_FLAGS "-std=c++17 -O3 ${CMAKE_CXX_FLAGS}")
else()
	set (CMAKE_CXX_FLAGS "-MMD -std=c++17 -Wall -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -g ${CMAKE_CXX_FLAGS}")
endif()

if(BUILD_TEST) 
	message(STATUS "Building Tests")
	subdirs(test)
endif()

