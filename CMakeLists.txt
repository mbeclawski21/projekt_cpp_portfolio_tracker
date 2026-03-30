cmake_minimum_required(VERSION 3.10)

project(Portfolio_tracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(TrackerPortfolio src/main.cpp)