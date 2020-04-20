############################################################
#   The CMake File for SDL2FireFrame.  It provides for
#   the following libraries:  Boost, GLM, GL, GLEW, pthread,
#   stdc++, SDL2
#   Created by Edward C. Eberle <eberdeed@ebedeed.net>
#   March 2020 San Diego California USA
#   Copyright Maureen Ann Quinn March 2020
############################################################
cmake_minimum_required(VERSION 2.6)
if (POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()
project(sdl2fireworks)
add_subdirectory(src)
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN YES)
set(PROJECT_NAME "SDL2 Fireworks")
set(PROJECT_BRIEF "A program to render a fireworks display.")
doxygen_add_docs(doc include src COMMENT "Generating documentation.")
install(DIRECTORY openglresources DESTINATION /usr/share FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY LGPL DESTINATION /usr/share/doc/sdl2fireworks-doc FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY build/html DESTINATION /usr/share/doc/sdl2fireworks-doc FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY build/man DESTINATION /usr/share/doc/sdl2fireworks-doc FILE_PERMISSIONS WORLD_READ)
install(FILES README.txt CHANGELOG.txt DESTINATION /usr/share/doc/sdl2fireworks-doc PERMISSIONS WORLD_READ)
