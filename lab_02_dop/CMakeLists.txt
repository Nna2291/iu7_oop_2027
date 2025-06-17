cmake_minimum_required(VERSION 3.13)

project(app.exe VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g2")

file(GLOB SOURCES src/hashmap/*.cpp)
file(GLOB HSOURCES src/collection/*.cpp)
file(GLOB SSOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${HSOURCES} ${SOURCES} ${SSOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    inc/
)


