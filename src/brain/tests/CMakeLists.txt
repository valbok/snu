include_directories(${snu_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(NeuronTest NeuronTest.cpp)
target_link_libraries(NeuronTest brain gtest)

add_executable(ColumnTest ColumnTest.cpp)
target_link_libraries(ColumnTest brain gtest)
