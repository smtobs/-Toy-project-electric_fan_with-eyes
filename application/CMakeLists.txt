cmake_minimum_required(VERSION 3.0)
project(smart_home1)

include_directories(
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/smart_home
)

set(CMAKE_CXX_COMPILER g++)
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/lcd.cpp
    src/keyPad.cpp
    src/systemManager.cpp)

target_link_libraries(
    ${PROJECT_NAME}
)
