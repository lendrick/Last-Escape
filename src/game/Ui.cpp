#include "globals.h"
#include "Input.h"
#include "Ui.h"

sf::Font fontUI;
int ui_focused_widget;
int ui_widget_id = 0;
int ui_setter = -1;

Widget *ui_base = NULL;
Widget *ui_energy = NULL;
Widget *ui_lives = NULL;
Widget *ui_hud = NULL;
Widget *ui_menu = NULL;
Widget *ui_pause = NULL;
Widget *ui_options = NULL;
Widget *ui_opctrls = NULL;
Widget *ui_controls[5];
Widget *ui_snd = NULL;
Widget *ui_popup = NULL;
void (*ui_popup_click)() = NULL;
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
	has_bg = true;
	txtPos = 0;
	sval = 0.f;
	slide = NULL;
	if (tp == UI_CONTAINER) {
		if (ui_base != NULL) {
			parent = ui_base;
		}
	}

	if (parent) {
		if (tp != UI_LABEL && tp != UI_TEXT) {
			background.SetImage(ui_background);
			if (tp == UI_CONTAINER) {
				setSize(203,203);
				background.SetSubRect(sf::IntRect(0,0,202,203));
			}else if (tp == UI_BUTTON) {
				setSize(75,20);
				background.SetSubRect(sf::IntRect(203,0,278,20));
			}else if (tp == UI_CHECK) {
				setSize(20,20);
				background.SetSubRect(sf::IntRect(203,40,223,60));
			}else if (tp == UI_HSLIDE) {
				setSize(150,3);
				background.SetSubRect(sf::IntRect(0,203,150,206));
				slider.SetImage(ui_background);
				slider.SetSubRect(sf::IntRect(203,40,223,60));
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

sf::String Widget::getText()
{
	return text;
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

void Widget::setSlide(void (*func)(float v))
{
	slide = func;
}

void Widget::setSlideValue(float v)
{
	if (type != UI_HSLIDE)
		return;

	sval = v;
	if (sval > 100.f)
		sval = 100.f;
	if (sval < 0.f)
		sval = 0.f;

	if (slide)
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

void Widget::setBg(const sf::Unicode::Text &Text)
{
	bgi.LoadFromFile(Text);
	background.SetImage(bgi);
	int w = (int)bgi.GetWidth();
	int h = (int)bgi.GetHeight();
	background.SetSubRect(sf::IntRect(0,0,w,h));
	setSize(w,h);
	setPos(320-(w/2),240-(h/2));
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
		if (type == UI_HSLIDE) {
			slider.SetPosition((x+(150.f*(sval/100.f))-10.f),y-11);
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
		float fx = 0;
		float fy = 0;
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

	if (id != ui_base->getId() && Event.Type != sf::Event::KeyPressed) {
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
				float v = (msx-x)/1.5f;
				setSlideValue(v);
			}
			if (type == UI_CHECK) {
				if (checked) {
					checked = false;
					background.SetSubRect(sf::IntRect(203,40,223,60));
				}else{
					checked = true;
					background.SetSubRect(sf::IntRect(223,40,243,60));
				}
			}
			return 1;
		}else if (type == UI_HSLIDE && id == ui_focused_widget) {
			if (Event.Type == sf::Event::MouseMoved) {
				float v = (msx-x)/1.5f;
				setSlideValue(v);
				//return 1;
			}else if (Event.Type == sf::Event::MouseButtonReleased) {
				ui_focused_widget = -1;
				//return 1;
			}
		}
	}

	if (id == ui_popup->id && Event.Type == sf::Event::KeyPressed)
		return 1;

	return 0;
}

bool ui_menuOpen()
{
	if (!ui_menu->isHidden() || !ui_pause->isHidden() || !ui_options->isHidden() || !ui_opctrls->isHidden())
		return true;

	return false;
}

void ui_start()
{
	g_player->init();
	const char* mapName = "subwaymap-new.tmx";
	game_map->loadMap(mapName);
	ui_menu->hide();
	ui_hud->show();
}

void ui_togglePause()
{
	if (ui_pause->isHidden()) {
		if (ui_menuOpen()) {
			ui_showMenu();
			return;
		}
		ui_pause->show();
		paused = true;
	}else{
		ui_pause->hide();
		paused = false;
	}
}

void ui_showMenu()
{
	ui_hud->hide();
	ui_pause->hide();
	ui_options->hide();
	ui_menu->show();
}

void ui_showOptions()
{
	ui_menu->hide();
	ui_opctrls->hide();
	ui_options->show();
}

void ui_showControls()
{
	ui_options->hide();
	ui_opctrls->show();
}

void ui_quit()
{
	App->Close();
}

void ui_setEvent(sf::Event &Event)
{
	if (Event.Type == sf::Event::KeyPressed)
	{
		char buf[255];
		inputs[ui_setter].key = Event.Key.Code;
		if (isalnum(Event.Key.Code)) {
			sprintf(buf,"%c",(char)Event.Key.Code);
		}else{
			switch (Event.Key.Code) {
			case sf::Key::LBracket:
				sprintf(buf,"(");
				break;
			case sf::Key::RBracket:
				sprintf(buf,")");
				break;
			case sf::Key::SemiColon:
				sprintf(buf,";");
				break;
			case sf::Key::Comma:
				sprintf(buf,",");
				break;
			case sf::Key::Period:
				sprintf(buf,".");
				break;
			case sf::Key::Quote:
				sprintf(buf,"\"");
				break;
			case sf::Key::Slash:
				sprintf(buf,"/");
				break;
			case sf::Key::BackSlash:
				sprintf(buf,"\\");
				break;
			case sf::Key::Tilde:
				sprintf(buf,"`");
				break;
			case sf::Key::Equal:
				sprintf(buf,"=");
				break;
			case sf::Key::Dash:
				sprintf(buf,"-");
				break;
			case sf::Key::Space:
				sprintf(buf,"Space");
				break;
			case sf::Key::Left:
				sprintf(buf,"Left Arrow");
				break;
			case sf::Key::Right:
				sprintf(buf,"Right Arrow");
				break;
			case sf::Key::Up:
				sprintf(buf,"Up Arrow");
				break;
			case sf::Key::Down:
				sprintf(buf,"Down Arrow");
				break;
			case sf::Key::Numpad0:
				sprintf(buf,"np-0");
				break;
			case sf::Key::Numpad1:
				sprintf(buf,"np-1");
				break;
			case sf::Key::Numpad2:
				sprintf(buf,"np-2");
				break;
			case sf::Key::Numpad3:
				sprintf(buf,"np-3");
				break;
			case sf::Key::Numpad4:
				sprintf(buf,"np-4");
				break;
			case sf::Key::Numpad5:
				sprintf(buf,"np-5");
				break;
			case sf::Key::Numpad6:
				sprintf(buf,"np-6");
				break;
			case sf::Key::Numpad7:
				sprintf(buf,"np-7");
				break;
			case sf::Key::Numpad8:
				sprintf(buf,"np-8");
				break;
			case sf::Key::Numpad9:
				sprintf(buf,"np-9");
				break;
			default:
				sprintf(buf,"???");
			}
		}
		ui_controls[ui_setter]->setText(buf);
		for (int i=0; i<5; i++) {
			if (i != ui_setter && inputs[i].key == Event.Key.Code) {
				ui_controls[i]->setText("???");
				inputs[i].key = sf::Key::Count;
			}
		}
		ui_setter = -1;
	}
}

void ui_popupImage(const sf::Unicode::Text &path, void (*func)())
{
	ui_popup->setBg(path);
	int w;
	int h;
	ui_popup->getSize(w,h);
	ui_popup->child->setSize(w,h);
	ui_popup_click = func;
	ui_popup->show();
}

void ui_hidePopup()
{
	ui_popup->hide();
	if (ui_popup_click)
		ui_popup_click();

	ui_popup_click = NULL;
}

void ui_setJump()
{
	char buf[255];
	ui_setter = INPUT_JUMP;
	sf::Unicode::Text txt = ui_controls[INPUT_JUMP]->getText().GetText();
	sprintf(buf,"-- %s --",static_cast<std::string>(txt).c_str());
	ui_controls[INPUT_JUMP]->setText(buf);
}

void ui_setLeft()
{
	char buf[255];
	ui_setter = INPUT_LEFT;
	sf::Unicode::Text txt = ui_controls[INPUT_LEFT]->getText().GetText();
	sprintf(buf,"-- %s --",static_cast<std::string>(txt).c_str());
	ui_controls[INPUT_LEFT]->setText(buf);
}

void ui_setRight()
{
	char buf[255];
	ui_setter = INPUT_RIGHT;
	sf::Unicode::Text txt = ui_controls[INPUT_RIGHT]->getText().GetText();
	sprintf(buf,"-- %s --",static_cast<std::string>(txt).c_str());
	ui_controls[INPUT_RIGHT]->setText(buf);
}

void ui_setCrouch()
{
	char buf[255];
	ui_setter = INPUT_CROUCH;
	sf::Unicode::Text txt = ui_controls[INPUT_CROUCH]->getText().GetText();
	sprintf(buf,"-- %s --",static_cast<std::string>(txt).c_str());
	ui_controls[INPUT_CROUCH]->setText(buf);
}

void ui_setShoot()
{
	char buf[255];
	ui_setter = INPUT_SHOOT;
	sf::Unicode::Text txt = ui_controls[INPUT_SHOOT]->getText().GetText();
	sprintf(buf,"-- %s --",static_cast<std::string>(txt).c_str());
	ui_controls[INPUT_SHOOT]->setText(buf);
}

void ui_setVol(float v)
{
	sf::Listener::SetGlobalVolume(v);
	char buf[255];
	sprintf(buf,"Volume: %.0f%%",v);
	ui_snd->setText(buf);
}

void ui_init()
{
	if (!fontUI.LoadFromFile("fonts/DejaVuSansMono.ttf"))
		printf("failed to load font\n");

	if (!ui_background.LoadFromFile("images/ui.png"))
		printf("failed to load ui sprites\n");

	// create the actual ui items
	ui_base = new Widget(UI_CONTAINER,ui_base);

	ui_hud = new Widget(UI_CONTAINER,ui_base);
	ui_hud->toggleBg();
	ui_hud->hide();
	ui_menu = new Widget(UI_CONTAINER,ui_base);
	ui_menu->setPos(220,140);
	ui_pause = new Widget(UI_CONTAINER,ui_base);
	ui_pause->setPos(220,140);
	ui_pause->hide();
	ui_options = new Widget(UI_CONTAINER,ui_base);
	ui_options->setPos(220,140);
	ui_options->hide();
	ui_opctrls = new Widget(UI_CONTAINER,ui_base);
	ui_opctrls->setPos(220,140);
	ui_opctrls->hide();

	ui_energy = new Widget(UI_LABEL,ui_hud);
	ui_energy->setSize(200,20);
	ui_energy->setPos(2,2);
	ui_lives = new Widget(UI_LABEL,ui_hud);
	ui_lives->setSize(200,20);
	ui_lives->setPos(2,22);

	ui_popup = new Widget(UI_CONTAINER,ui_base);
	ui_popup->setPos(220,140);
	ui_popup->hide();

	// main menu
	Widget *b = new Widget(UI_LABEL,ui_menu);
	b->setText("Xeon");
	b->setPos(62,10);

	b = new Widget(UI_BUTTON,ui_menu);
	b->setText("Start");
	b->setPos(62,60);
	b->setClick(ui_start);

	b = new Widget(UI_BUTTON,ui_menu);
	b->setText("Options");
	b->setPos(62,90);
	b->setClick(ui_showOptions);

	b = new Widget(UI_BUTTON,ui_menu);
	b->setText("Quit");
	b->setPos(62,120);
	b->setClick(ui_quit);

	// popup
	b = new Widget(UI_BUTTON,ui_popup);
	b->toggleBg();
	b->setClick(ui_hidePopup);

	// pause menu
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
	b->setClick(ui_quit);

	// options menu
	b = new Widget(UI_LABEL,ui_options);
	b->setText("Xeon Options");
	b->setPos(62,10);

	b = new Widget(UI_LABEL,ui_options);
	char buf[255];
	float svol = sf::Listener::GetGlobalVolume()*100.f;
	sprintf(buf,"Volume: %.0f%%",svol);
	b->setText(buf);
	b->setPos(62,50);
	ui_snd = b;

	b = new Widget(UI_HSLIDE,ui_options);
	b->setSlideValue(svol);
	b->setPos(28,78);
	b->setSlide(ui_setVol);

	b = new Widget(UI_BUTTON,ui_options);
	b->setText("Controls");
	b->setPos(62,110);
	b->setClick(ui_showControls);

	b = new Widget(UI_BUTTON,ui_options);
	b->setText("Back");
	b->setPos(62,170);
	b->setClick(ui_showMenu);

	// controls menu
	b = new Widget(UI_LABEL,ui_opctrls);
	b->setText("Xeon Controls");
	b->setPos(62,10);

	b = new Widget(UI_LABEL,ui_opctrls);
	b->setText("Move Left:");
	b->setPos(10,40);
	ui_controls[INPUT_LEFT] = new Widget(UI_BUTTON,ui_opctrls);
	ui_controls[INPUT_LEFT]->setText("Left Arrow");
	ui_controls[INPUT_LEFT]->toggleBg();
	ui_controls[INPUT_LEFT]->setPos(110,40);
	ui_controls[INPUT_LEFT]->setClick(ui_setLeft);

	b = new Widget(UI_LABEL,ui_opctrls);
	b->setText("Move Right:");
	b->setPos(10,65);
	ui_controls[INPUT_RIGHT] = new Widget(UI_BUTTON,ui_opctrls);
	ui_controls[INPUT_RIGHT]->setText("Right Arrow");
	ui_controls[INPUT_RIGHT]->toggleBg();
	ui_controls[INPUT_RIGHT]->setPos(110,65);
	ui_controls[INPUT_RIGHT]->setClick(ui_setRight);

	b = new Widget(UI_LABEL,ui_opctrls);
	b->setText("Jump:");
	b->setPos(10,90);
	ui_controls[INPUT_JUMP] = new Widget(UI_BUTTON,ui_opctrls);
	ui_controls[INPUT_JUMP]->setText("Up Arrow");
	ui_controls[INPUT_JUMP]->toggleBg();
	ui_controls[INPUT_JUMP]->setPos(110,90);
	ui_controls[INPUT_JUMP]->setClick(ui_setJump);

	b = new Widget(UI_LABEL,ui_opctrls);
	b->setText("Shoot:");
	b->setPos(10,115);
	ui_controls[INPUT_SHOOT] = new Widget(UI_BUTTON,ui_opctrls);
	ui_controls[INPUT_SHOOT]->setText("Space");
	ui_controls[INPUT_SHOOT]->toggleBg();
	ui_controls[INPUT_SHOOT]->setPos(110,115);
	ui_controls[INPUT_SHOOT]->setClick(ui_setShoot);

	b = new Widget(UI_LABEL,ui_opctrls);
	b->setText("Crouch:");
	b->setPos(10,140);
	ui_controls[INPUT_CROUCH] = new Widget(UI_BUTTON,ui_opctrls);
	ui_controls[INPUT_CROUCH]->setText("Down Arrow");
	ui_controls[INPUT_CROUCH]->toggleBg();
	ui_controls[INPUT_CROUCH]->setPos(110,140);
	ui_controls[INPUT_CROUCH]->setClick(ui_setCrouch);

	b = new Widget(UI_BUTTON,ui_opctrls);
	b->setText("Back");
	b->setPos(62,170);
	b->setClick(ui_showOptions);

	// set the default game events
	inputs[INPUT_JUMP].key = sf::Key::Up;
	inputs[INPUT_LEFT].key = sf::Key::Left;
	inputs[INPUT_RIGHT].key = sf::Key::Right;
	inputs[INPUT_CROUCH].key = sf::Key::Down;
	inputs[INPUT_SHOOT].key = sf::Key::Space;
}

int ui_event(sf::Event &Event)
{
	if (ui_setter > -1) {
		ui_setEvent(Event);
		return 1;
	}
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
		ui_energy->setTextColor(0x01, 135, 0x00);
	ui_energy->setText(buf);

	sprintf(buf, "Lives: %d", player.lifes);
	if (player.lifes < 2)
		ui_lives->setTextColor(0xef, 0x29, 0x29);
	else
		ui_lives->setTextColor(0x01, 135, 0x00);
	ui_lives->setText(buf);

	ui_base->draw();
}

void ui_exit()
{
	delete ui_base;
}
