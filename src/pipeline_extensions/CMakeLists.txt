file(GLOB vc_extensions ingress/*.cpp egress/*.cpp)

add_library(vic_extensions STATIC ${vc_extensions})

target_link_libraries(vic_extensions vic_core ${OpenCV_LIBS} ${Boost_LIBRARIES})
