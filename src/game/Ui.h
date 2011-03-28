/*
 *  This file is part of Last Escape.
 *
 *  Last Escape is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
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

	void setPos(float px, float py);
	void getPos(float &px, float &py);

	void setText(const sf::Unicode::Text &Text);
	sf::String getText();
	void setTextColor(int r, int g, int b);
	void setTextSize(int sz);

	bool toggleBg();
	void setBg(const sf::Unicode::Text &Text);

	void setClick(void (*func)());
	void setSlide(void (*func)(float v));
	void setSlideValue(float v);

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
	sf::Image bgi;
	sf::Sprite background;
	sf::Sprite slider;
	sf::String text;

	Widget *next;
	Widget *parent;

	void (*click)();
	void (*slide)(float v);
	char txt[255];
	int txtPos;
	int id;
	int type;
	int size;
	int height, width;
	float pos_x, pos_y;
	float sval;
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
void ui_popupImage(const sf::Unicode::Text &path, void (*func)());
void ui_render(Player &player);
void ui_exit(void);
void ui_start();
void ui_togglePause();
void ui_showMenu();
void ui_showOptions();
void ui_quit();
bool ui_menuOpen();
