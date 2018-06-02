add_executable(test_ProcessorBase test_ProcessorBase.cpp)

target_link_libraries(test_ProcessorBase viclib ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_test(ProcessorBase test_ProcessorBase)
