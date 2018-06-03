add_executable(test_ProcessorBase test_ProcessorBase.cpp)
add_executable(test_ProcessorInstance test_ProcessorInstance.cpp)
add_executable(test_ProcessorContext test_ProcessorContext.cpp)

target_link_libraries(test_ProcessorBase viclib ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_link_libraries(test_ProcessorInstance viclib ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_link_libraries(test_ProcessorContext viclib ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_test(ProcessorBase test_ProcessorBase)
add_test(ProcessorInstance test_ProcessorInstance)
add_test(ProcessorContext test_ProcessorContext)
