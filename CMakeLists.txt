cmake_minimum_required(VERSION 3.17)
project(allegro_pinpoint)

# C Flags
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 11)

# C++ Flags
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)

# Allegro Libs
include_directories(/usr/local/Cellar/allegro/5.2.6.0/include)
link_directories(/usr/local/Cellar/allegro/5.2.6.0//lib)
file(GLOB LIBRARIES "/usr/local/Cellar/allegro/5.2.6.0/lib/*.dylib")

# Setting up Targets
add_executable(allegro_pinpoint_cpp
                    src/main.cpp
                    src/city.cpp)

target_link_libraries(allegro_pinpoint_cpp ${LIBRARIES})