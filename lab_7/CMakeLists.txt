cmake_minimum_required(VERSION 2.9)
project(seventh)

#find_package(MPI) #make it REQUIRED, if you want
#include_directories(SYSTEM ${MPI_INCLUDE_PATH}) 
#target_link_libraries(${PROJECT_NAME} ${MPI_C_LIBRARIES})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread")


set(SRC_LIST main.cpp timer.hpp add_func.hpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

# MPI
find_package(MPI REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC MPI::MPI_CXX)
