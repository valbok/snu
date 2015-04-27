include_directories(${snu_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(CElegansTest CElegansTest.cpp)
target_link_libraries(CElegansTest worm brain gtest)

add_executable(teachFoodDirection teachFoodDirection.cpp)
target_link_libraries(teachFoodDirection worm brain)
