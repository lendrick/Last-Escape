cmake_minimum_required (VERSION 2.8)

#set up the project
project (Xeonergy)

#tell cmake where to search for package finding modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#find the required libraries
#not sure whether these are required:
#find_package (Freetype REQUIRED)
#find_package (Sndfile REQUIRED)
#find_package (OpenAL REQUIRED)
find_package (SFML 1.6 COMPONENTS system window graphics audio REQUIRED)
include_directories (${SFML_INCLUDE_DIR})

#adding the game subdirectory
add_subdirectory (game)
