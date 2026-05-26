cmake_minimum_required(VERSION 3.14)
project(TrackerPortfolio)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

message(STATUS "Pobieranie biblioteki JSON...")
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

message(STATUS "Pobieranie biblioteki CPR...")
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 1.11.0)
FetchContent_MakeAvailable(cpr)

message(STATUS "Pobieranie biblioteki GLFW...")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
FetchContent_MakeAvailable(glfw)

message(STATUS "Pobieranie Dear ImGui...")
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG v1.90.4)
FetchContent_Populate(imgui)

message(STATUS "Pobieranie ImPlot...")
FetchContent_Declare(implot GIT_REPOSITORY https://github.com/epezent/implot.git GIT_TAG v0.16)
FetchContent_Populate(implot)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMPLOT_DIR ${implot_SOURCE_DIR})

file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

file(GLOB IMPLOT_SOURCES
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

add_executable(TrackerPortfolio 
    src/main.cpp 
    src/ui.cpp 
    src/CoinGeckoApi.cpp
    src/PortfolioStorage.cpp
    ${IMGUI_SOURCES} 
    ${IMPLOT_SOURCES}
)

target_include_directories(TrackerPortfolio PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${IMPLOT_DIR}
    ${glfw_SOURCE_DIR}/include
    include
)

target_link_libraries(TrackerPortfolio PRIVATE 
    cpr::cpr 
    nlohmann_json::nlohmann_json
    glfw
)

if(WIN32)
    target_link_libraries(TrackerPortfolio PRIVATE opengl32 gdi32 shell32 user32)
endif()