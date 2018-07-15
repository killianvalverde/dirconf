cmake_minimum_required(VERSION 3.15)

project(dirconf)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

include_directories(/usr/include/glib-2.0)
include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include)

set(SOURCE_FILES
        src/file_managers.hpp
        src/main.cpp
        src/program.hpp
        src/program.cpp
)

add_executable(dirconf ${SOURCE_FILES})
target_link_libraries(dirconf -lglib-2.0 -lgio-2.0 -lspeed -lstdc++fs)
install(TARGETS dirconf DESTINATION bin)
