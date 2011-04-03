/*
 *  This file is part of Last Escape.
 *
 *  Last Escape is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Last Escape is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Last Escape.  If not, see <http://www.gnu.org/licenses/>.
 */


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
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
sf::Music backgroundMusic;
bool enableMusic = true;
std::string startMap;
bool debugMode = false;

sf::View uiView(sf::FloatRect(0, 0, 640, 480));
sf::View gameView(sf::FloatRect(0, 480, 640, 0));

ImageCache imageCache;
SoundCache soundCache;

bool paused = false;
const double time_step = 1.0f/60.0f;


void update(double dt) {
	// Update the physics
	static const int steps = 3;
	for(int i=0; i<steps; i++){
		//cpSpaceStep(game_map->physSpace, dt/(cpFloat)steps);
		cpSpaceStep(game_map->physSpace, dt/(cpFloat)steps);
	}

	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		(*it)->doUpdate(dt);
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
		} else if((*tmp)->toTeleport) {
			(*tmp)->doTeleport();
		}
	}
}

double frand(double lower, double upper) {
	return ((upper-lower)*((double)rand()/RAND_MAX))+lower;
}

double deg2rad(double deg) {
	return deg * 180.0f / M_PI;
}

double rad2deg(double rad) {
  return rad * M_PI / 180.0f;
}

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
  srand((unsigned)time(0));
	startMap = "desert_map.tmx";
	bool fullScreen = false;

	// Parse a few command-line arguments
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--disable-music") == 0)
			enableMusic = false;
		else if (strcmp(argv[i], "--map") == 0)
			startMap = argv[++i];
		else if (strcmp(argv[i], "--iddqd") == 0)
			godMode = true;
		else if (strcmp(argv[i], "--fullscreen") == 0)
			fullScreen = true;
		else if (strcmp(argv[i], "--debug") == 0)
			debugMode = true;
	}

	// Create main window
	if(fullScreen) {
		App = new sf::RenderWindow(sf::VideoMode(640, 480), "Last Escape", 		sf::Style::Fullscreen|sf::Style::Close);
	} else {
		App = new sf::RenderWindow(sf::VideoMode(640, 480), "Last Escape", 		sf::Style::Close);
		App->SetPosition((sf::VideoMode::GetDesktopMode().Width/2)-320, (sf::VideoMode::GetDesktopMode().Height/2)-260);
	}

	App->SetFramerateLimit(1.0f/time_step);
	App->UseVerticalSync(true);

	cpInitChipmunk();

	if (!fontUI.LoadFromFile("fonts/orbitron-bold.otf"))
		printf("failed to load font\n");

	//g_player = new Player(0,0);

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

		double ElapsedTime = Clock.GetElapsedTime();
		Clock.Reset();

		// Clamp frame update time if worse than 20fps, so it'll slow down instead
		// of just getting very jerky (which breaks jump heights)
		double frameTime = std::min(ElapsedTime, 0.05);

		// Clear screen
		App->Clear();

		if(game_map != NULL && game_map->isLoaded()) {
			// This function loads a new map if one has been set with SetNextMap.
			// Due to physics functions, we can't switch maps mid-loop.
			cleanup();
			game_map->loadNextMap();
			
			if(!paused) {
				//update(frameTime);
				update(time_step);
			}

			game_map->renderLandscape();
			App->SetView(gameView);
			game_map->renderBackground();
			renderActors();
			game_map->renderForeground();
		}

		App->SetView(uiView);

		ui_render(g_player);

		// Finally, display the rendered frame on screen
		App->Display();
	}

	delete game_map;
	delete App;

	ui_exit();

	return EXIT_SUCCESS;
}
