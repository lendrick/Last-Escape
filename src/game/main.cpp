
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
#include "AnimatedActor.h"
#include "TempPlayer.h"
#include "Player.h"

list<Actor *> actors;
Map * game_map;
sf::RenderWindow *App;
Player *g_player;

sf::Font fontUI;

void update(Player& player, float dt) {
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		(*it)->update(dt);

	// Check for collisions with the player
	float px1, py1, px2, py2;
	player.getBoundingBox(px1, py1, px2, py2);
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		float x1, y1, x2, y2;
		(*it)->getBoundingBox(x1, y1, x2, y2);
		if (px2 < x1 || x2 < px1 || py2 < y1 || y2 < py1)
			continue;
		(*it)->collidePlayer(player);
	}
}

void renderActors() {
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		(*it)->draw();
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

void renderUI(Player& player) {
	char buf[256];
	float energy = 100*player.energy/player.energy_max;
	sprintf(buf, "Energy: %.0f%%", energy);
	sf::String textEnergy(buf, fontUI, 12);
	if (energy < 20.f)
		textEnergy.SetColor(sf::Color(0xef, 0x29, 0x29));
	else
		textEnergy.SetColor(sf::Color(0xed, 0xd4, 0x00));
	textEnergy.Move(8, 8);
	App->Draw(textEnergy);
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

	if (!fontUI.LoadFromFile("fonts/DejaVuSansMono.ttf"))
		printf("failed to load font\n");
	

	// Create game objects
	game_map = new Map();
	Player p1;
	g_player = &p1;

	sf::Clock Clock;

	// Create Animation test
	sf::Image xeon;
	xeon.LoadFromFile("images/xeon.png");
	AnimatedActor testActor(xeon);

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

			if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape)
				App->Close();

			if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Up)
				p1.jump();

			if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Space)
				p1.shoot();
		}

		float ElapsedTime = Clock.GetElapsedTime();
		Clock.Reset();

		// Clear screen
		App->Clear();
		update(p1, ElapsedTime);
		cleanup();

		p1.logic(ElapsedTime);

		game_map->renderBackground();
		p1.render();
		renderActors();
		game_map->renderForeground();

		testActor.update(ElapsedTime);
		testActor.draw();

		renderUI(p1);

		// Finally, display the rendered frame on screen
		App->Display();
	}
	
	delete game_map;
	delete App;
	
	return EXIT_SUCCESS;
}
