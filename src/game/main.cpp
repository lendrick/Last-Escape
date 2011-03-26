
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <list>
#include "Map.h"
#include "globals.h"

using namespace std;

list<Actor *> actors;
Map * map;

/// This function cleans up deleted actors.
void cleanup() {
}

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
	// Create main window
	sf::RenderWindow App(sf::VideoMode(640, 480), "SFML Graphics");
	const sf::Input& input = App.GetInput();

	// Create game objects
	map = new Map(App);

	// Start game loop
	while (App.IsOpened())
	{
		// Process events
		sf::Event Event;
		while (App.GetEvent(Event))
		{
			// Close window : exit
			if (Event.Type == sf::Event::Closed)
				App.Close();
		}

		// Clear screen
		App.Clear();

		// TEMP: scrolling camera
		if (input.IsKeyDown(sf::Key::Left))  map->cam_x--;
		if (input.IsKeyDown(sf::Key::Right)) map->cam_x++;
		if (input.IsKeyDown(sf::Key::Up))    map->cam_y--;
		if (input.IsKeyDown(sf::Key::Down))  map->cam_y++;

		cleanup();
		
		// Draw Map
		map->render();

		// Finally, display the rendered frame on screen
		App.Display();
	}
	
	delete m;

	return EXIT_SUCCESS;
}
