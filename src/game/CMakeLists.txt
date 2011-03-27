#xeonergy game

#variable for the loop
#note that looping isn't recommended, as cmake won't be able to recognize whether it has to update the makefile
# but this way it'll be faster and easier to expand

set (folders
		.
		tinyxml
		#add all subdirectories here, one per line
		)

foreach (folder ${folders})

	#search this folder for source files
	file (GLOB srcs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${folder}/*.cpp")

	#store sourcefile into the variable we gonna use
	list (APPEND game_srcs ${srcs})

	#store the include directories
	list (APPEND game_incs "${CMAKE_CURRENT_SOURCE_DIR}/${folder}")

endforeach(folder)

#ugly, but we need to get those stuff into this scope
set (game_srcs ${game_srcs} PARENT_SCOPE)
set (game_incs ${game_incs} PARENT_SCOPE)

#include the exstracted include dirs
include_directories (${game_incs})

#add our executable
add_executable (xeonergy ${game_srcs})

set_target_properties (xeonergy PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../..")

#linking to libraries
target_link_libraries (xeonergy ${SFML_LIBRARIES})
