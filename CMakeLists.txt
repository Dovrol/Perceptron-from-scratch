cmake_minimum_required(VERSION 3.15)
project(Perceptron)

set(CMAKE_CXX_STANDARD 14)

#find_package(Boost 1.40.0 COMPONENTS filesystem system iostreams REQUIRED)
include_directories(include)

add_executable(Perceptron main.cpp)
#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

