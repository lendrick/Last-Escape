
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Map.h"

using namespace std;

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
	Map m(App);

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
		if (input.IsKeyDown(sf::Key::Left))  m.cam_x--;
		if (input.IsKeyDown(sf::Key::Right)) m.cam_x++;
		if (input.IsKeyDown(sf::Key::Up))    m.cam_y--;
		if (input.IsKeyDown(sf::Key::Down))  m.cam_y++;

		
		// Draw Map
		m.render();

		// Finally, display the rendered frame on screen
		App.Display();
	}

	return EXIT_SUCCESS;
}
