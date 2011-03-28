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

#include "Input.h"
#include "globals.h"
#include "Ui.h"

InputItem inputs[5];

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
	inputCrouch = false;
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

		if (ui_event(Event))
			continue;

		if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape)
			ui_togglePause();

		if (!ui_menuOpen()) {
			if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == inputs[INPUT_JUMP].key)
				inputJump = true;

			if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == inputs[INPUT_SHOOT].key)
				inputShoot = true;
			
			if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == inputs[INPUT_CROUCH].key)
				inputCrouch = true;
		}
	}

	if (!ui_menuOpen()) {
		const sf::Input& appInput = App->GetInput();

		if(appInput.IsKeyDown(inputs[INPUT_LEFT].key) && !appInput.IsKeyDown(inputs[INPUT_RIGHT].key)) {
			inputDirection = FACING_LEFT;
		} else if(appInput.IsKeyDown(inputs[INPUT_RIGHT].key)) {
			inputDirection = FACING_RIGHT;
		} else if(appInput.IsKeyDown(inputs[INPUT_CROUCH].key)) {
			inputCrouch = true;
		}
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

bool Input::crouch() {
	return inputCrouch;
}

bool Input::shooting()
{
	if (ui_menuOpen())
		return false;
	return inputShoot || App->GetInput().IsKeyDown(inputs[INPUT_SHOOT].key);
}

bool Input::jumping()
{
	if (ui_menuOpen())
		return false;
	return inputJump || App->GetInput().IsKeyDown(inputs[INPUT_JUMP].key);
}

bool Input::crouching()
{
	if (ui_menuOpen())
		return false;
	return inputCrouch || App->GetInput().IsKeyDown(inputs[INPUT_CROUCH].key);
}
