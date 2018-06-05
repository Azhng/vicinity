add_executable(simple_video_stream simple_video_stream.cpp)

target_link_libraries(simple_video_stream vic_core vic_extensions ${OpenCV_LIBS} ${Boost_LIBRARIES})
