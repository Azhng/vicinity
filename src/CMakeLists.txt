file(GLOB v_src pipeline_core/*.cpp)

add_library(viclib STATIC ${v_src})

add_executable(vicinity main/main.cpp)

target_link_libraries(vicinity viclib ${OpenCV_LIBS} ${Boost_LIBRARIES})
