add_executable(simple_video_stream simple_video_stream/simple_video_stream.cpp)
add_executable(hot_swap_processor hot_swap_processor/hot_swap_processor.cpp)

target_link_libraries(simple_video_stream vic_core vic_extensions ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_link_libraries(hot_swap_processor vic_core vic_extensions ${OpenCV_LIBS} ${Boost_LIBRARIES})
