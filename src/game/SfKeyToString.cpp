/////////////////////////////////////////////////////////////////////////////
// SfKeyToString
// Copyright (c) 2011 Anton Kornelsen
/////////////////////////////////////////////////////////////////////////////
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
// Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
//
// Everyone is permitted to copy and distribute verbatim or modified
// copies of this license document, and changing it is allowed as long
// as the name is changed.
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.
/////////////////////////////////////////////////////////////////////////////
// This program is free software. It comes without any warranty, to
// the extent permitted by applicable law. You can redistribute it
// and/or modify it under the terms of the Do What The Fuck You Want
// To Public License, Version 2, as published by Sam Hocevar. See
// http://sam.zoy.org/wtfpl/COPYING for more details.
/////////////////////////////////////////////////////////////////////////////


#include "SfKeyToString.h"

std::string sfKeyToString(sf::Keyboard::Key& code)
{
	return translation[code];
}

sf::Keyboard::Key sfKeyFromString(std::string str)
{
	for (std::map<sf::Keyboard::Key, std::string>::iterator it= translation.begin(); it != translation.end(); ++it)
	{
		if(it->second == str)
		{
			return it->first;
		}
	}
	return sf::Keyboard::KeyCount;
}

std::string sfKeyToString(sf::Event::KeyEvent& key)
{
	std::stringstream ss;
	if(key.Alt)     { ss << "Alt "; }
	if(key.Control) { ss << "Ctrl "; }
	if(key.Shift)   { ss << "Shift "; }
	ss << translation[key.Code];
	return ss.str();
}

void initSfKeyToString()
{
	translation[sf::Keyboard::Key::A] = "A";
	translation[sf::Keyboard::Key::B] = "B";
	translation[sf::Keyboard::Key::C] = "C";
	translation[sf::Keyboard::Key::D] = "D";
	translation[sf::Keyboard::Key::E] = "E";
	translation[sf::Keyboard::Key::F] = "F";
	translation[sf::Keyboard::Key::G] = "G";
	translation[sf::Keyboard::Key::H] = "H";
	translation[sf::Keyboard::Key::I] = "I";
	translation[sf::Keyboard::Key::J] = "J";
	translation[sf::Keyboard::Key::K] = "K";
	translation[sf::Keyboard::Key::L] = "L";
	translation[sf::Keyboard::Key::M] = "M";
	translation[sf::Keyboard::Key::N] = "N";
	translation[sf::Keyboard::Key::O] = "O";
	translation[sf::Keyboard::Key::P] = "P";
	translation[sf::Keyboard::Key::Q] = "Q";
	translation[sf::Keyboard::Key::R] = "R";
	translation[sf::Keyboard::Key::S] = "S";
	translation[sf::Keyboard::Key::T] = "T";
	translation[sf::Keyboard::Key::U] = "U";
	translation[sf::Keyboard::Key::V] = "V";
	translation[sf::Keyboard::Key::W] = "W";
	translation[sf::Keyboard::Key::X] = "X";
	translation[sf::Keyboard::Key::Y] = "Y";
	translation[sf::Keyboard::Key::Z] = "Z";
	translation[sf::Keyboard::Key::Num0] = "0";
	translation[sf::Keyboard::Key::Num1] = "1";
	translation[sf::Keyboard::Key::Num2] = "2";
	translation[sf::Keyboard::Key::Num3] = "3";
	translation[sf::Keyboard::Key::Num4] = "4";
	translation[sf::Keyboard::Key::Num5] = "5";
	translation[sf::Keyboard::Key::Num6] = "6";
	translation[sf::Keyboard::Key::Num7] = "7";
	translation[sf::Keyboard::Key::Num8] = "8";
	translation[sf::Keyboard::Key::Num9] = "9";
	translation[sf::Keyboard::Key::Escape] = "Escape";
	translation[sf::Keyboard::Key::LControl] = "LControl";
	translation[sf::Keyboard::Key::LShift] = "LShift";
	translation[sf::Keyboard::Key::LAlt] = "LAlt";
	translation[sf::Keyboard::Key::LSystem] = "LSystem";
	translation[sf::Keyboard::Key::RControl] = "RControl";
	translation[sf::Keyboard::Key::RShift] = "RShift";
	translation[sf::Keyboard::Key::RAlt] = "RAlt";
	translation[sf::Keyboard::Key::RSystem] = "RSystem";
	translation[sf::Keyboard::Key::Menu] = "Menu";
	translation[sf::Keyboard::Key::LBracket] = "LBracket";
	translation[sf::Keyboard::Key::RBracket] = "RBracket";
	translation[sf::Keyboard::Key::SemiColon] = ";";
	translation[sf::Keyboard::Key::Comma] = ",";
	translation[sf::Keyboard::Key::Period] = ".";
	translation[sf::Keyboard::Key::Quote] = "'";
	translation[sf::Keyboard::Key::Slash] = "/";
	translation[sf::Keyboard::Key::BackSlash] = "\\";
	translation[sf::Keyboard::Key::Tilde] = "~";
	translation[sf::Keyboard::Key::Equal] = "=";
	translation[sf::Keyboard::Key::Dash] = "-";
	translation[sf::Keyboard::Key::Space] = "Space";
	translation[sf::Keyboard::Key::Return] = "Return";
	translation[sf::Keyboard::Key::Back] = "Back";
	translation[sf::Keyboard::Key::Tab] = "Tab";
	translation[sf::Keyboard::Key::PageUp] = "PageUp";
	translation[sf::Keyboard::Key::PageDown] = "PageDown";
	translation[sf::Keyboard::Key::End] = "End";
	translation[sf::Keyboard::Key::Home] = "Home";
	translation[sf::Keyboard::Key::Insert] = "Insert";
	translation[sf::Keyboard::Key::Delete] = "-";
	translation[sf::Keyboard::Key::Add] = "+";
	translation[sf::Keyboard::Key::Subtract] = "-";
	translation[sf::Keyboard::Key::Multiply] = "*";
	translation[sf::Keyboard::Key::Divide] = "/";
	translation[sf::Keyboard::Key::Left] = "Left Arrow";
	translation[sf::Keyboard::Key::Right] = "Right Arrow";
	translation[sf::Keyboard::Key::Up] = "Up Arrow";
	translation[sf::Keyboard::Key::Down] = "Down Arrow";
	translation[sf::Keyboard::Key::Numpad0] = "Numpad0";
	translation[sf::Keyboard::Key::Numpad1] = "Numpad1";
	translation[sf::Keyboard::Key::Numpad2] = "Numpad2";
	translation[sf::Keyboard::Key::Numpad3] = "Numpad3";
	translation[sf::Keyboard::Key::Numpad4] = "Numpad4";
	translation[sf::Keyboard::Key::Numpad5] = "Numpad5";
	translation[sf::Keyboard::Key::Numpad6] = "Numpad6";
	translation[sf::Keyboard::Key::Numpad7] = "Numpad7";
	translation[sf::Keyboard::Key::Numpad8] = "Numpad8";
	translation[sf::Keyboard::Key::Numpad9] = "Numpad9";
	translation[sf::Keyboard::Key::F1] = "F1";
	translation[sf::Keyboard::Key::F2] = "F2";
	translation[sf::Keyboard::Key::F3] = "F3";
	translation[sf::Keyboard::Key::F4] = "F4";
	translation[sf::Keyboard::Key::F5] = "F5";
	translation[sf::Keyboard::Key::F6] = "F6";
	translation[sf::Keyboard::Key::F7] = "F7";
	translation[sf::Keyboard::Key::F8] = "F8";
	translation[sf::Keyboard::Key::F9] = "F9";
	translation[sf::Keyboard::Key::F10] = "F10";
	translation[sf::Keyboard::Key::F11] = "F11";
	translation[sf::Keyboard::Key::F12] = "F12";
	translation[sf::Keyboard::Key::F13] = "F13";
	translation[sf::Keyboard::Key::F14] = "F14";
	translation[sf::Keyboard::Key::F15] = "F15";
	translation[sf::Keyboard::Key::Pause] = "Pause";
	translation[sf::Keyboard::Key::KeyCount] = "Unknown key";
}
