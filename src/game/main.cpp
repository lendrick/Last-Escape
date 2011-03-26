
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <list>
//#include <boost/foreach.hpp>
#include "globals.h"

#include "Map.h"
#include "Actor.h"
#include "TempPlayer.h"


list<Actor *> actors;
Map * game_map;
sf::RenderWindow *App;


/// This function calls update() on all the actors, including (eventually) the player
void update() {
//	foreach(Actor * actor, actors) {
//		actor->update();
//	}
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
	App = new sf::RenderWindow(sf::VideoMode(640, 480), "SFML Graphics");
	App->UseVerticalSync(true);
	const sf::Input& input = App->GetInput();

	// Create game objects
	game_map = new Map();
	TempPlayer p1;

	// Start game loop
	while (App->IsOpened())
	{
		// Process events
		sf::Event Event;
		while (App->GetEvent(Event))
		{
			// Close window : exit
			if (Event.Type == sf::Event::Closed)
				App->Close();
		}

		// Clear screen
		App->Clear();
		update();
		cleanup();


		p1.logic();

		game_map->renderBackground();
		p1.render();
		game_map->renderForeground();

		// Finally, display the rendered frame on screen
		App->Display();
	}
	
	delete game_map;
	delete App;
	
	return EXIT_SUCCESS;
}
