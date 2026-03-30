cmake_minimum_required(VERSION 3.14...3.30)
project(TrackerPortfolio)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

message(STATUS "Pobieranie biblioteki JSON...")
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

message(STATUS "Pobieranie biblioteki CPR (C++ Requests)...")
# Używam nowszej wersji (1.11.0 zamiast 1.10.4)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 1.11.0)
FetchContent_MakeAvailable(cpr)

add_executable(TrackerPortfolio src/main.cpp)

target_link_libraries(TrackerPortfolio PRIVATE cpr::cpr nlohmann_json::nlohmann_json)