include_directories(${snu_SOURCE_DIR})
include_directories(${snu_SOURCE_DIR}/lib/enki)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(SnuPuckTest SnuPuckTest.cpp)
target_link_libraries(SnuPuckTest brain robots gtest enki)
