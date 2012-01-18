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

#include "Widget.h"

sf::Font Widget::fontUI;
int Widget::ui_focused_widget;
int Widget::ui_widget_id = 0;

Widget::Widget(int tp, Widget *par) {
	type = tp;
	id = ui_widget_id++;
	hidden = false;
	child = NULL;
	parent = par;
	next = NULL;
	click = NULL;
	anyKey = NULL;
	checked = false;
	has_bg = true;
	txtPos = 0;
	sval = 0.f;
	slide = NULL;
	if (tp == UI_CONTAINER) {
//		if (ui_base != NULL) {
//			parent = ui_base;
//		}
	}

	if (parent) {
		if (tp != UI_LABEL && tp != UI_TEXT) {

			sf::Texture* ui_background = imageCache["ui.png"];
			if (!ui_background)
			{
				ui_background = imageCache.getFailsafeTexture();
				printf("failed to load ui sprites\n");
			}
			else
				ui_background->SetSmooth(false);


			background.SetTexture(*ui_background);
			if (tp == UI_CONTAINER) {
				setSize(203,203);
				background.SetTextureRect(sf::IntRect(0,0,202,203));
			}else if (tp == UI_BUTTON) {
				setSize(75,20);
				background.SetTextureRect(sf::IntRect(203,0,75,20));
			}else if (tp == UI_CHECK) {
				setSize(20,20);
				background.SetTextureRect(sf::IntRect(203,40,20,20));
			}else if (tp == UI_PBAR) {
				setSize(150,32);
				background.SetTextureRect(sf::IntRect(0,207,150,32));
			}else if (tp == UI_HSLIDE) {
				setSize(150,3);
				background.SetTextureRect(sf::IntRect(0,203,150,3));
				slider.SetTexture(*ui_background);
				slider.SetTextureRect(sf::IntRect(203,40,20,20));
			}
		}
		if (parent->child) {
			Widget *n = parent->child;
			while (n->next) {
				n = n->next;
			}
			n->next = this;
		}else{
			parent->child = this;
		}
	}
	setPos(0, 0);
	text.SetFont(fontUI);
	setTextColor(255,255,255);
	setTextSize(12);
}

Widget::~Widget() {
	if (type == UI_CONTAINER) {
		Widget *n = child;
		Widget *c;
		while (n) {
			c = n;
			n = n->next;
			delete c;
		}
	}
}

int Widget::getId()
{
	return id;
}

void Widget::setPos(double px, double py) {
	pos_x = px;
	pos_y = py;
	if (type != UI_CONTAINER) {
		double x = 0;
		double y = 0;
		parent->getPos(x,y);
		px += x;
		py += y;
	}

	background.SetPosition(px, py);
}

void Widget::getPos(double &px, double &py) {
	px = pos_x;
	py = pos_y;
}

void Widget::setTextColor(int r, int g, int b)
{
	text.SetColor(sf::Color(r,g,b, 255)); // TODO give alpha
}

void Widget::setText(const sf::String &Text)
{
	text.SetString(Text);
}

sf::String Widget::getText()
{
	return text.GetString();
}

void Widget::setTextSize(int sz)
{
	size = sz;
	text.SetCharacterSize((double)size);
}

void Widget::setClick(void (*func)())
{
	click = func;
}

void Widget::setAnyKey(void (*func)())
{
	anyKey = func;
}

void Widget::setSlide(void (*func)(double v))
{
	slide = func;
}

void Widget::setSlideValue(double v)
{
	sval = v;
	if (sval > 100.f)
		sval = 100.f;
	if (sval < 0.f)
		sval = 0.f;

	if (type == UI_HSLIDE && slide)
		slide(sval);
}

void Widget::show()
{
	hidden = false;
}

void Widget::hide()
{
	hidden = true;
}

bool Widget::isHidden()
{
	return hidden;
}

bool Widget::toggleBg()
{
	if (has_bg) {
		has_bg = false;
		return true;
	}else{
		has_bg = true;
		return false;
	}
}

void Widget::setBg(const sf::String &Text)
{
	bgi.LoadFromFile(Text.ToAnsiString());
//	bgi.SetSmooth(false);
	background.SetTexture(bgi);
	int w = (int)bgi.GetWidth();
	int h = (int)bgi.GetHeight();
	background.SetTextureRect(sf::IntRect(0,0,w,h));
	setSize(w,h);
	setPos(320.0f - (w / 2.0f), 240.0f - (h / 2.0f));
}

bool Widget::isChecked()
{
	if (type != UI_CHECK)
		return false;

	return checked;
}

void Widget::setSize(int w, int h) {
	height = h;
	width = w;
}

void Widget::getSize(int &w, int &h) {
	h = height;
	w = width;
}

void Widget::draw() {
	if (hidden)
		return;
	if (has_bg && type != UI_LABEL && type != UI_TEXT)
		App->Draw(background);
	if (type == UI_CONTAINER) {
		Widget *n = child;
		while (n) {
			n->draw();
			n = n->next;
		}
	}else{
		double x = 0;
		double y = 0;
//		sf::FloatRect p = text.GetRect();
		sf::FloatRect p = text.GetGlobalBounds();
		parent->getPos(x,y);
		if (type == UI_BUTTON || type == UI_PBAR) {
			x += (pos_x+(width/2))-(p.Width/2);
		}else{
			x += pos_x;
		}
		y += pos_y+3;
		if (type == UI_PBAR)
			y += 3;
		text.SetPosition(x,y);
		parent->getPos(x,y);
		x += pos_x;
		y += pos_y;
		if (type == UI_PBAR) {
			sf::RectangleShape percentageBarShape(sf::Vector2f(x+(130.f*(sval/100.f)), 13.f));
			percentageBarShape.SetPosition(sf::Vector2f(x+10, y+7));

			if(sval > 60.f)
				percentageBarShape.SetFillColor(sf::Color(0xef, 0x29, 0x29));
			else
				percentageBarShape.SetFillColor(sf::Color(0xff, 165, 0x0));

			App->Draw(percentageBarShape);
//			sf::Shape Line;
//			if (sval > 60.f) {
//				Line = sf::Shape::Line(x+10.f, y+13.f, (x+(132.f*(sval/100.f)))+10, y+13.f, 12, sf::Color(0xef, 0x29, 0x29));
//			}else{
//				Line = sf::Shape::Line(x+10.f, y+13.f, (x+(132.f*(sval/100.f)))+10, y+13.f, 12, sf::Color(0xff, 165, 0x0));
//			}
//			App->Draw(Line);
		}
		App->Draw(text);
		if (type == UI_HSLIDE) {
			slider.SetPosition((x+(150.f*(sval/100.f))-10.f),y-8);
			App->Draw(slider);
		}
	}
}

int Widget::event(sf::Event &Event)
{
	if (hidden)
		return 0;

	int x = (int)pos_x;
	int y = (int)pos_y;
	if (type != UI_CONTAINER) {
		double fx = 0;
		double fy = 0;
		parent->getPos(fx,fy);
		x += (int)fx;
		y += (int)fy;
	}
	int mx = (int)x+width;
	int my = (int)y+height;
	int msx = 0;
	int msy = 0;
	if (Event.Type == sf::Event::MouseButtonPressed || Event.Type == sf::Event::MouseButtonReleased) {
		msx = Event.MouseButton.X;
		msy = Event.MouseButton.Y;
	}else if (Event.Type == sf::Event::MouseMoved) {
		msx = Event.MouseMove.X;
		msy = Event.MouseMove.Y;
	}

//	if (id != ui_base->getId() && Event.Type != sf::Event::KeyPressed) {
	if (Event.Type != sf::Event::KeyPressed) {
		if (type == UI_HSLIDE) {
			if (msx < x || msx > mx || msy < y-10 || msy > my+9)
				return 0;
		}else if (msx < x || msx > mx || msy < y || msy > my) {
			return 0;
		}
	}
	if (type == UI_CONTAINER) {
		Widget *n = child;
		int r = 0;
		while (n && !r) {
			r = n->event(Event);
			n = n->next;
		}
		return r;
	}else if (type != UI_LABEL) {
		if (Event.Type == sf::Event::MouseButtonPressed) {
			ui_focused_widget = id;
			if (click)
				click();
			if (type == UI_HSLIDE) {
				double v = (msx-x)/1.5f;
				setSlideValue(v);
			}
			if (type == UI_CHECK) {
				if (checked) {
					checked = false;
					background.SetTextureRect(sf::IntRect(203,40,223,60));
				}else{
					checked = true;
					background.SetTextureRect(sf::IntRect(223,40,243,60));
				}
			}
			return 1;
		}else if (type == UI_HSLIDE && id == ui_focused_widget) {
			if (Event.Type == sf::Event::MouseMoved) {
				double v = (msx-x)/1.5f;
				setSlideValue(v);
				//return 1;
			}else if (Event.Type == sf::Event::MouseButtonReleased) {
				ui_focused_widget = -1;
				//return 1;
			}
		}
	}

	if (Event.Type == sf::Event::KeyPressed && anyKey)
		this->anyKey();

//	if (id == ui_popup->id && Event.Type == sf::Event::KeyPressed)
	if (type != UI_CONTAINER && Event.Type == sf::Event::KeyPressed)
		return 1;

	return 0;
}
