
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <list>
#include <boost/foreach.hpp>
#include "Map.h"
#include "Actor.h"
#include "globals.h"

list<Actor *> actors;
Map * game_map;

/// This function calls update() on all the actors, including (eventually) the player
void update() {
	foreach(Actor * actor, actors) {
		actor->update();
	}
}

/// This function cleans up deleted actors.
void cleanup() {
	if(actors.empty())
		return;
	
	list<Actor *>::iterator i = actors.end();
	list<Actor *>::iterator tmp;
	
	i--;
	
	while(i != actors.begin()) {
		tmp = i;
		i--;
		
		if((*tmp)->isDestroyed()) {
			delete *tmp;
			actors.erase(tmp);
		}
	}
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

		update();
		
		cleanup();
		
		// Draw Map
		game_map->render();

		// Finally, display the rendered frame on screen
		App.Display();
	}
	
	delete game_map;

	return EXIT_SUCCESS;
}
