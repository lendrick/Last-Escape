cmake_minimum_required (VERSION 2.8)

#set up the project
project (LastEscape)

#tell cmake where to search for package finding modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#Variables to control the build type
SET(CMAKE_BUILD_TYPE debug)
#SET(CMAKE_BUILD_TYPE distribution)
SET(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -Wextra -Wno-unused")
SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-Wall")

if(UNIX)
  link_libraries(m)
endif(UNIX)

#find the required libraries
find_package (SFML 1.6 COMPONENTS system window graphics audio REQUIRED)
include_directories (${SFML_INCLUDE_DIR})

#adding the physics library
add_subdirectory (3rdparty/chipmunk-physics)
include_directories (3rdparty/chipmunk-physics/include)

#adding the game subdirectory
add_subdirectory (game)
