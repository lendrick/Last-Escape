
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
#include "Player.h"
#include "Input.h"
#include "Sound.h"

list<Actor *> actors;
Map * game_map;
sf::RenderWindow *App;
Player *g_player;
Input input;

sf::Font fontUI;

Sound * backgroundMusic = new Sound("01 Game-Game_0.ogg");
Sound * fireSound = new Sound("shoot.ogg");
Sound * damageSound = new Sound();
Sound * deathSound = new Sound();
Sound * bulletHitSound = new Sound();
Sound * enemyDeathSound = new Sound();


void update(Player& player, float dt) {
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		(*it)->update(dt);
}

void renderActors() {
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		(*it)->draw();
}

/// This function cleans up deleted actors.
void cleanup() {
	if(actors.empty())
		return;
	
	list<Actor *>::iterator i = actors.begin();
	list<Actor *>::iterator tmp;
		
	while(i != actors.end()) {
		tmp = i;
		++i;
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

	if (!fontUI.LoadFromFile("fonts/DejaVuSansMono.ttf"))
		printf("failed to load font\n");
	

	// Create game objects
	game_map = new Map();
	Player p1;
	g_player = &p1;
	game_map->setCameraFollow(g_player);

	sf::Clock Clock;

	// Create Animation test
	sf::Image xeon;
	xeon.LoadFromFile("images/xeon.png");
	backgroundMusic->playSound();
	
	// Start game loop
	while (App->IsOpened())
	{
		input.poll();
		if(input.quit())
			App->Close();

		float ElapsedTime = Clock.GetElapsedTime();
		Clock.Reset();

		// Clear screen
		App->Clear();
		update(p1, ElapsedTime);
		cleanup();

		game_map->renderBackground();
		renderActors();
		game_map->renderForeground();

		renderUI(p1);

		// Finally, display the rendered frame on screen
		App->Display();
	}
	
	delete game_map;
	delete App;
	
	return EXIT_SUCCESS;
}
