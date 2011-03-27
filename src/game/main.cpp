
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
#include "StartPoint.h"

list<Actor *> actors;
Map * game_map;
sf::RenderWindow *App;
Player *g_player = 0;
Input input;
bool godMode = false;
Sound * backgroundMusic = NULL;
bool enableMusic = true;
std::string startMap;

ImageCache imageCache;
SoundCache soundCache;

bool paused = false;


void update(float dt) {
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		(*it)->update(dt);
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
	startMap = "subwaymap-new.tmx";

	// Parse a few command-line arguments
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--disable-music") == 0)
			enableMusic = false;
		else if (strcmp(argv[i], "--map") == 0)
			startMap = argv[++i];
		else if (strcmp(argv[i], "--iddqd") == 0)
			godMode = true;
	}

	// Create main window
	App = new sf::RenderWindow(sf::VideoMode(640, 480), "Xeonergy", sf::Style::Close);
	App->SetPosition((sf::VideoMode::GetDesktopMode().Width/2)-320, 
		(sf::VideoMode::GetDesktopMode().Height/2)-260);
	App->SetFramerateLimit(60);
	App->UseVerticalSync(true);
	
	g_player = new Player();
	
	if (!fontUI.LoadFromFile("fonts/DejaVuSansMono.ttf"))
		printf("failed to load font\n");

	// Create game objects
	// DON'T LOAD A REAL MAP HERE!  ui_start does that.
	game_map = new Map("");

	/*
	if (enableMusic)
	{
		soundCache["01 Game-Game_0.ogg"]->setLoop(true);
		soundCache["01 Game-Game_0.ogg"]->playSound();
	}
	*/
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
		
		if(game_map != NULL && game_map->isLoaded()) {
			if(!paused) {
				update(frameTime);
			}
			
			cleanup();

			game_map->renderLandscape();
			game_map->renderBackground();
			renderActors();
			game_map->renderForeground();
		}

		ui_render(*g_player);

		// Finally, display the rendered frame on screen
		App->Display();
	}

	delete game_map;
	delete App;

	ui_exit();

	return EXIT_SUCCESS;
}
