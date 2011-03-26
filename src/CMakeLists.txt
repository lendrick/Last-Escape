#find the required libraries
find_package (SFML 1.6 COMPONENTS system window graphics audio REQUIRED)
include_directories (${SFML_INCLUDE_DIR})

#adding the game subdirectory
add_subdirectory (game)
