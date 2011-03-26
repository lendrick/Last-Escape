
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <list>
#include "Map.h"
#include "globals.h"

std::list<Actor *> actors;
Map * game_map;

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
	App.UseVerticalSync(true);
	const sf::Input& input = App.GetInput();

	// Create game objects
	game_map = new Map(App);

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
		if (input.IsKeyDown(sf::Key::Left))  game_map->cam_x--;
		if (input.IsKeyDown(sf::Key::Right)) game_map->cam_x++;
		if (input.IsKeyDown(sf::Key::Up))    game_map->cam_y--;
		if (input.IsKeyDown(sf::Key::Down))  game_map->cam_y++;

		
		// Draw Map
		game_map->render();

		// Finally, display the rendered frame on screen
		App.Display();
	}
	
	delete game_map;

	return EXIT_SUCCESS;
}
