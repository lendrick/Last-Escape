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
	inputDirection = Facing::None;
	inputJump = false;
	inputShoot = false;
	inputQuit = false;
	inputCrouch = false;
	inputStopJump = false;
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
			if (Event.Type == sf::Event::KeyPressed) {
				if(Event.Key.Code == inputs[INPUT_JUMP].key)
					inputJump = true;
				if (Event.Key.Code == inputs[INPUT_SHOOT].key)
					inputShoot = true;
				if (Event.Key.Code == inputs[INPUT_CROUCH].key)
					inputCrouch = true;
			} else if(Event.Type == sf::Event::JoyButtonPressed 
				&& Event.JoyButton.JoystickId == 0) {
				if(Event.JoyButton.Button == 0) {
					inputShoot = true;
				} else if(Event.JoyButton.Button == 1) {
					inputJump = true;
				}
			} else if(Event.Type == sf::Event::KeyReleased) {
				if(Event.Key.Code == inputs[INPUT_JUMP].key)
					inputStopJump = true;
			} else if (Event.Type == sf::Event::JoyButtonReleased
				&& Event.JoyButton.JoystickId == 0) {
				if(Event.JoyButton.Button == 1) {
					inputStopJump = true;
				}
			}
		}
		/*
		if(Event.Type == sf::Event::JoyButtonPressed) {
			cout << "Button: " << Event.JoyButton.JoystickId << 
				": " << Event.JoyButton.Button << "\n";
		}
		*/
	}

	if (!ui_menuOpen()) {
		const sf::Input& appInput = App->GetInput();

		if(appInput.IsKeyDown(inputs[INPUT_LEFT].key) && !appInput.IsKeyDown(inputs[INPUT_RIGHT].key)) {
			inputDirection = Facing::Left;
		} else if(appInput.IsKeyDown(inputs[INPUT_RIGHT].key)) {
			inputDirection = Facing::Right;
		} 
		
		double xAxis = appInput.GetJoystickAxis(0, sf::Joy::AxisX);
		double yAxis = appInput.GetJoystickAxis(0, sf::Joy::AxisY);
		
		if(xAxis > 50) 
			inputDirection = Facing::Right;
		else if(xAxis < -50)
			inputDirection = Facing::Left;
		
		if(yAxis > 50)
			inputCrouch = true;
		
		
		if(appInput.IsKeyDown(inputs[INPUT_CROUCH].key)) {
			inputCrouch = true;
		}
		
		if(appInput.IsJoystickButtonDown(0, 0)) {
			inputShoot = true;
		}
		
		if(appInput.IsJoystickButtonDown(0, 1)) {
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
	return inputShoot || App->GetInput().IsKeyDown(inputs[INPUT_SHOOT].key) || App->GetInput().IsJoystickButtonDown(0, 0);
}

bool Input::jumping()
{
	if (ui_menuOpen())
		return false;
	return inputJump || App->GetInput().IsKeyDown(inputs[INPUT_JUMP].key) || App->GetInput().IsJoystickButtonDown(0, 1);
}

bool Input::stopJump() {
	if (ui_menuOpen())
		return false;
	
	return inputStopJump;
}

bool Input::crouching()
{
	if (ui_menuOpen())
		return false;
	return inputCrouch || App->GetInput().IsKeyDown(inputs[INPUT_CROUCH].key) || App->GetInput().GetJoystickAxis(0, sf::Joy::AxisY) > 50;
}
