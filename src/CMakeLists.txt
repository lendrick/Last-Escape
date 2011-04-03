cmake_minimum_required (VERSION 2.8)

#set up the project
project (LastEscape)

#tell cmake where to search for package finding modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#add windows dependencies
if (WIN32)
	if (NOT DEFINED WINDEPS)
		set (WINDEPS "${CMAKE_SOURCE_DIR}/windeps")
	endif()
	set (SFMLDIR "${WINDEPS}/SFML-1.6")
endif(WIN32)

#Variables to control the build type
SET(CMAKE_BUILD_TYPE debug)
#SET(CMAKE_BUILD_TYPE distribution)
SET(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -Wextra -Wno-unused -pg")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "-pg")
SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "-pg")
SET(CMAKE_MODULE_LINKER_FLAGS_DEBUG "-pg")
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
#install windows dependencies
if (WIN32)
	install (FILES 
	"${SFMLDIR}/lib/sfml-audio.dll" 
	"${SFMLDIR}/lib/sfml-graphics.dll"
	"${SFMLDIR}/lib/sfml-system.dll"
	"${SFMLDIR}/lib/sfml-window.dll"
	"${SFMLDIR}/extlibs/bin/libsndfile-1.dll"
	"${SFMLDIR}/extlibs/bin/openal32.dll" 
	DESTINATION ${CMAKE_SOURCE_DIR}/..)
endif(WIN32)

#adding the game subdirectory
add_subdirectory (game)
