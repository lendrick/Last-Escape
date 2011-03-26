#include "Input.h"
#include "globals.h"

Input::Input() {
	initFrame();
}

Input::~Input() {
}

void Input::initFrame() {
	inputDirection = FACING_NONE;
	inputJump = false;
	inputShoot = false;
	inputQuit = false;
}

void Input::poll() {
	// Process events
	initFrame();
	sf::Event Event;
	
	
	while (App->GetEvent(Event))
	{
		// Close window : exit
		if (Event.Type == sf::Event::Closed)
			inputQuit = true;
		
		if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape)
			inputQuit = true;
		
		if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Up)
			inputJump = true;
		
		if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Space)
			inputShoot = true;
	}

	const sf::Input& appInput = App->GetInput();

	if(appInput.IsKeyDown(sf::Key::Left) && !appInput.IsKeyDown(sf::Key::Right)) {
		inputDirection = FACING_LEFT;
	} else if(appInput.IsKeyDown(sf::Key::Right)) {
		inputDirection = FACING_RIGHT;
	}
}

int Input::direction() 
{
	return inputDirection;
}

bool Input::jump()
{
	return inputJump; 
}

bool Input::quit()
{
	return inputQuit;
}

bool Input::shoot()
{
	return inputShoot;
}

bool Input::shooting()
{
	return App->GetInput().IsKeyDown(sf::Key::Space);
}

bool Input::jumping() 
{
	return App->GetInput().IsKeyDown(sf::Key::Up);
}	