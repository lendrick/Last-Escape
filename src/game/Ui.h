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

#pragma once
#include "globals.h"
#include "Widget.h"
#include "Input.h"
#include "SFML/System.hpp"
#include "Player.h"
#include "SoundCache.h"
#include "SfKeyToString.h"
#include <stdio.h>

extern sf::Font fontUI;
extern int focused_widget;
extern Widget *ui_base;
extern Widget *ui_energy;
extern Widget *ui_hud;
extern Widget *ui_menu;
extern Widget *ui_pause;

void ui_init(void);
int ui_event(sf::Event &Event);
void ui_popupImage(const sf::String &path, void (*func)());
void ui_render(Player * p);
void ui_exit(void);
void ui_start();
void ui_togglePause();
void ui_showMenu();
void ui_showOptions();
void ui_showCredits();
void ui_quit();
extern bool ui_menuOpen();
