cmake_minimum_required(VERSION 3.26)
project(hex)

set(CMAKE_CXX_STANDARD 20)

add_executable(hex main.cpp
        Table.cpp
        Table.h
        Block.cpp
        Block.h
        Selector.cpp
        Selector.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/SFML/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories("C:/SFML/SFML-2.6.1/include")
target_link_libraries(hex sfml-system sfml-window sfml-graphics sfml-audio)