include_directories(${snu_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(mempot mempot.cpp)
target_link_libraries(mempot brain)

add_executable(memosc memosc.cpp)
target_link_libraries(memosc brain)

add_executable(CElegansTest CElegansTest.cpp)
target_link_libraries(CElegansTest brain gtest)
