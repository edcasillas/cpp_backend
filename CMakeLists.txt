cmake_minimum_required(VERSION 3.0.0)
project(cpp_backend VERSION 0.1.0)
INCLUDE_DIRECTORIES(
    D:/dev/vcpkg/installed/x64-windows/include
    ${CMAKE_SOURCE_DIR}/include
)

LINK_DIRECTORIES(
    D:/dev/vcpkg/installed/x64-windows/lib
)

add_executable(cpp_backend main.cpp)

TARGET_LINK_LIBRARIES(cpp_backend
    sqlite3 restbed
)
