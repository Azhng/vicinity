file(GLOB vc_extensions ingress/*.cpp egress/*.cpp transform/*.cpp)

add_library(vic_extensions SHARED ${vc_extensions})

target_link_libraries(vic_extensions vic_core ${OpenCV_LIBS} ${Boost_LIBRARIES})
