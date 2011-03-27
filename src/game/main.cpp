
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
#include "Ui.h"
#include "ImageCache.h"
#include "SoundCache.h"

list<Actor *> actors;
Map * game_map;
sf::RenderWindow *App;
Player *g_player;
Input input;

ImageCache imageCache;
SoundCache soundCache;

bool paused = false;


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

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	bool enableMusic = true;
	const char* mapName = "subwaymap.tmx";

	// Parse a few command-line arguments
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--disable-music") == 0)
			enableMusic = false;
		else if (strcmp(argv[i], "--map") == 0)
			mapName = argv[++i];
	}

	// Create main window
	App = new sf::RenderWindow(sf::VideoMode(640, 480), "SFML Graphics");
	App->SetFramerateLimit(60);
	App->UseVerticalSync(true);

	if (!fontUI.LoadFromFile("fonts/DejaVuSansMono.ttf"))
		printf("failed to load font\n");

	// Create game objects
	game_map = new Map(mapName);
	Player p1;
	g_player = &p1;
	game_map->setCameraFollow(g_player);


	if (enableMusic)
		soundCache["01 Game-Game_0.ogg"]->playSound();

	ui_init();

	sf::Clock Clock;
	Clock.Reset();
	
	// Start game loop
	while (App->IsOpened())
	{
		input.poll();
		if(input.quit())
			App->Close();

		float ElapsedTime = Clock.GetElapsedTime();
		Clock.Reset();

		// Clamp frame update time if worse than 20fps, so it'll slow down instead
		// of just getting very jerky (which breaks jump heights)
		float frameTime = std::min(ElapsedTime, 0.05f);

		// Clear screen
		App->Clear();
		
		if(!paused) 
			update(p1, frameTime);
		
		cleanup();

		game_map->renderLandscape();
		game_map->renderBackground();
		renderActors();
		game_map->renderForeground();

		ui_render(p1);

		// Finally, display the rendered frame on screen
		App->Display();
	}

	delete game_map;
	delete App;

	ui_exit();

	return EXIT_SUCCESS;
}
