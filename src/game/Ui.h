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
#include "SFML/System.hpp"
#include "Player.h"
#include <stdio.h>

#define UI_CONTAINER	0
#define UI_BUTTON	1
#define UI_LABEL	2
#define UI_TEXT		3
#define UI_CHECK	4
#define UI_HSLIDE	5
#define UI_PBAR		6

class Widget {
public:
	Widget(int tp, Widget *parent);
	~Widget();

	void setPos(double px, double py);
	void getPos(double &px, double &py);

	void setText(const sf::String &Text);
	sf::String getText();
	void setTextColor(int r, int g, int b);
	void setTextSize(int sz);

	bool toggleBg();
	void setBg(const sf::String &Text);

	void setClick(void (*func)());
	void setSlide(void (*func)(double v));
	void setSlideValue(double v);
	void setAnyKey(void (*func)());

	void show();
	void hide();
	bool isHidden();
	bool isChecked();

	int getId();

	void draw();

	void setSize(int w, int h);
	void getSize(int &w, int &h);

	int event(sf::Event &Event);
	Widget *child;

protected:
	sf::Texture bgi;
	sf::Sprite background;
	sf::Sprite slider;
	sf::Text text;

	Widget *next;
	Widget *parent;

	void (*click)();
	void (*slide)(double v);
	void (*anyKey)();
	char txt[255];
	int txtPos;
	int id;
	int type;
	int size;
	int height, width;
	double pos_x, pos_y;
	double sval;
	bool checked;
	bool hidden;
	bool has_bg;
};

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
void ui_quit();
bool ui_menuOpen();
