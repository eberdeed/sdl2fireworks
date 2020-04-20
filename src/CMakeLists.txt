############################################################
#   The CMake File for SDL2FireFrame.  It provides for
#   the following libraries:  Boost, GLM, GL, GLEW, pthread,
#   stdc++, SDL2
#   Created by Edward C. Eberle <eberdeed@ebedeed.net>
#   March 2020 San Diego California USA
#   Copyright Maureen Ann Quinn March 2020
############################################################
cmake_minimum_required(VERSION 2.6)
project(sdl2fireworks)
add_executable(sdl2fireworks shader.cpp sdl2fireworks.cpp camera.cpp)
add_definitions(-g -fPIC -pthread -std=c++17 -pthread)
include_directories(/usr/local/include/SDL2 /usr/include/GL
/usr/include/glm /usr/include/boost)
link_directories(/usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu)
target_link_libraries(sdl2fireworks stdc++ GL GLEW SDL2-2.0 pthread 
boost_filesystem boost_system pthread)
install(TARGETS sdl2fireworks DESTINATION /usr/bin)
