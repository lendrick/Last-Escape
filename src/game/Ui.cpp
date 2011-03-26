#include "globals.h"
#include "Ui.h"

sf::Font fontUI;
int ui_focused_widget;
int ui_widget_id = 0;

Widget *ui_base = NULL;
Widget *ui_energy = NULL;
Widget *ui_hud = NULL;
Widget *ui_menu = NULL;
Widget *ui_pause = NULL;
sf::Image ui_background;

Widget::Widget(int tp, Widget *par) {
	type = tp;
	id = ui_widget_id++;
	hidden = false;
	child = NULL;
	parent = par;
	next = NULL;
	click = NULL;
	checked = false;
	txtPos = 0;
	if (tp == UI_CONTAINER) {
		if (ui_base != NULL) {
			parent = ui_base;
		}
	}

	if (parent) {
		if (tp != UI_LABEL && tp != UI_TEXT && ui_hud) {
			background.SetImage(ui_background);
			if (tp == UI_CONTAINER) {
				setSize(203,203);
				background.SetSubRect(sf::IntRect(0,0,202,203));
			}else if (tp == UI_BUTTON) {
				setSize(75,20);
				background.SetSubRect(sf::IntRect(202,0,277,20));
			}else if (tp == UI_CHECK) {
				setSize(20,20);
				background.SetSubRect(sf::IntRect(202,40,222,60));
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

void Widget::setPos(float px, float py) {
	pos_x = px;
	pos_y = py;
	if (type != UI_CONTAINER) {
		float x = 0;
		float y = 0;
		parent->getPos(x,y);
		px += x;
		py += y;
	}

	background.SetPosition(px, py);
}

void Widget::getPos(float &px, float &py) {
	px = pos_x;
	py = pos_y;
}

void Widget::setTextColor(int r, int g, int b)
{
	text.SetColor(sf::Color(r,g,b));
}

void Widget::setText(const sf::Unicode::Text &Text)
{
	text.SetText(Text);
}

void Widget::setTextSize(int sz)
{
	size = sz;
	text.SetSize((float)size);
}

void Widget::setClick(void (*func)())
{
	click = func;
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

bool Widget::isChecked()
{
	if (type != UI_CHECK)
		return false;

	return checked;
}

void Widget::setSize(int w, int h) {
	height = h;
	width = w;
	//background.Resize((float)w,(float)h);
}

void Widget::getSize(int &w, int &h) {
	h = height;
	w = width;
}

void Widget::draw() {
	if (hidden)
		return;
	App->Draw(background);
	if (type == UI_CONTAINER) {
		Widget *n = child;
		while (n) {
			n->draw();
			n = n->next;
		}
	}else{
		float x = 0;
		float y = 0;
		sf::FloatRect p = text.GetRect();
		parent->getPos(x,y);
		if (type == UI_BUTTON) {
			x += (pos_x+(width/2))-(p.GetWidth()/2);
		}else{
			x += pos_x;
		}
		y += pos_y+3;
		text.SetPosition(x,y);
		App->Draw(text);
	}
}

int Widget::event(sf::Event &Event)
{
	if (hidden)
		return 0;
	if (id != ui_base->getId() && Event.Type != sf::Event::KeyPressed) {
		if (Event.Type != sf::Event::MouseButtonPressed)
			return 0;
		int x = (int)pos_x;
		int y = (int)pos_y;
		if (type != UI_CONTAINER) {
			float fx = 0;
			float fy = 0;
			parent->getPos(fx,fy);
			x += (int)fx;
			y += (int)fy;
		}
		int mx = (int)x+width;
		int my = (int)y+height;
		int msx = Event.MouseButton.X;
		int msy = Event.MouseButton.Y;
		if (!(msx > x && msx < mx && msy > y && msy < my))
			return 0;
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
			if (type == UI_CHECK) {
				if (checked) {
					checked = false;
					background.SetSubRect(sf::IntRect(202,40,222,60));
				}else{
					checked = true;
					background.SetSubRect(sf::IntRect(222,40,242,60));
				}
			}
			return 1;
		}else if (id == ui_focused_widget && Event.Type == sf::Event::KeyPressed) {
			if (type == UI_TEXT) {
				// Ok, I have nfi here should update the text, somehow
				//if (Event.Key.Code == sf::Key::Backspace) {
				//}else if
				txt[txtPos] = 0;
				setText(txt);
			}
		}
	}
	return 0;
}

void ui_start()
{
	ui_menu->hide();
	ui_hud->show();
}

void ui_togglePause()
{
	if (ui_pause->isHidden()) {
		ui_pause->show();
	}else{
		ui_pause->hide();
	}
}

void ui_showMenu()
{
	ui_hud->hide();
	ui_pause->hide();
	ui_menu->show();
}

void ui_quit()
{
	App->Close();
}

void ui_init()
{
	if (!fontUI.LoadFromFile("fonts/DejaVuSansMono.ttf"))
		printf("failed to load font\n");

	if (!ui_background.LoadFromFile("images/ui.png"))
		printf("failed to load ui sprites\n");

	ui_base = new Widget(UI_CONTAINER,ui_base);

	ui_hud = new Widget(UI_CONTAINER,ui_base);
	ui_menu = new Widget(UI_CONTAINER,ui_base);
	ui_menu->setPos(220,140);
	ui_pause = new Widget(UI_CONTAINER,ui_base);
	ui_pause->setPos(220,140);
	ui_pause->hide();

	ui_energy = new Widget(UI_LABEL,ui_hud);
	ui_energy->setSize(200,20);
	ui_energy->setPos(8,8);

	Widget *b = new Widget(UI_LABEL,ui_menu);
	b->setText("Xeon");
	b->setPos(62,10);

	b = new Widget(UI_BUTTON,ui_menu);
	b->setText("Start");
	b->setPos(62,60);
	b->setClick(ui_start);

	b = new Widget(UI_BUTTON,ui_menu);
	b->setText("Quit");
	b->setPos(62,90);
	b->setClick(ui_quit);

	b = new Widget(UI_LABEL,ui_pause);
	b->setText("Xeon (Paused)");
	b->setPos(62,10);

	b = new Widget(UI_BUTTON,ui_pause);
	b->setText("Continue");
	b->setPos(62,60);
	b->setClick(ui_togglePause);

	b = new Widget(UI_BUTTON,ui_pause);
	b->setText("Quit");
	b->setPos(62,90);
	b->setClick(ui_showMenu);
}

int ui_event(sf::Event &Event)
{
	int r = ui_base->event(Event);
	if (!r) {
		if (Event.Type == sf::Event::MouseButtonPressed)
			ui_focused_widget = 0;
	}

	if (!r && (!ui_menu->isHidden() || !ui_pause->isHidden()))
		r = 1;

	return r;
}

void ui_render(Player& player)
{
	char buf[256];
	float energy = 100*player.energy/player.energy_max;
	sprintf(buf, "Energy: %.0f%%", energy);
	if (energy < 20.f)
		ui_energy->setTextColor(0xef, 0x29, 0x29);
	else
		ui_energy->setTextColor(0xed, 0xd4, 0x00);
	ui_energy->setText(buf);

	ui_base->draw();
}

void ui_exit()
{
	delete ui_base;
}
