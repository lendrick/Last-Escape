#pragma once
#include "globals.h"
#include "SFML/System.hpp"
#include "Player.h"

extern sf::Font fontUI;
extern int focused_widget;

#define UI_CONTAINER	0
#define UI_BUTTON	1
#define UI_LABEL	2
#define UI_TEXT		3
#define UI_CHECK	4

class Widget {
public:
	Widget(int tp, Widget *parent);
	~Widget();

	void setPos(float px, float py);
	void getPos(float &px, float &py);

	void setText(const sf::Unicode::Text &Text);
	void setTextColor(int r, int g, int b);
	void setTextSize(int sz);

	void setClick(void (*func)());

	void show();
	void hide();
	bool isHidden();
	bool isChecked();

	int getId();

	void draw();

	void setSize(int w, int h);
	void getSize(int &w, int &h);

	int event(sf::Event &Event);

protected:
	sf::Sprite background;
	sf::String text;

	Widget *next;
	Widget *child;
	Widget *parent;

	void (*click)();
	char txt[255];
	int txtPos;
	int id;
	int type;
	int size;
	int height, width;
	float pos_x, pos_y;
	bool checked;
	bool hidden;
};

void ui_init(void);
int ui_event(sf::Event &Event);
void ui_render(Player &player);
void ui_exit(void);
void ui_start();
void ui_togglePause();
void ui_showMenu();
void ui_quit();
