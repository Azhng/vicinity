file(GLOB vc_core pipeline_core/*.cpp)

add_library(vic_core STATIC ${vc_core})

target_link_libraries(vicinity vic_core ${OpenCV_LIBS} ${Boost_LIBRARIES})

subdirs(pipeline_extensions)
