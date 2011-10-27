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

#include "Ui.h"

int ui_setter = -1;

Widget *ui_base = NULL;
Widget *ui_energy = NULL;
Widget *ui_lives = NULL;
Widget *ui_energyballs = NULL;
Widget *ui_hud = NULL;
Widget *ui_menu = NULL;
Widget *ui_pause = NULL;
Widget *ui_options = NULL;
Widget *ui_opctrls = NULL;
Widget *ui_controls[5];
Widget *ui_snd = NULL;
Widget *ui_popup = NULL;
void (*ui_popup_close)() = NULL;

bool ui_menuOpen()
{
	if (!ui_menu->isHidden() || !ui_pause->isHidden() || !ui_options->isHidden() || !ui_opctrls->isHidden())
		return true;

	return false;
}

void ui_start()
{
	if(!g_player) g_player = new Player(0, 0);
	g_player->init();
	ui_base->toggleBg();
	//const char* mapName = "subwaymap-new.tmx";
//	game_map->loadMap(mapName);
	game_map->loadMap(startMap);
	ui_menu->hide();
	ui_hud->show();
	gamestatus = Gamestatus::ingame;
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
	if (ui_base->toggleBg())
		ui_base->toggleBg();
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
		inputs[ui_setter].key = Event.Key.Code;
		ui_controls[ui_setter]->setText(sfKeyToString(Event.Key));
		for (int i=0; i<5; i++) {
			if (i != ui_setter && inputs[i].key == Event.Key.Code) {
				ui_controls[i]->setText("???");
				inputs[i].key = sf::Keyboard::KeyCount;
			}
		}
		ui_setter = -1;
	}
}

void ui_popupImage(const sf::String &path, void (*func)())
{
	ui_popup->setBg(path);
	int w;
	int h;
	ui_popup->getSize(w,h);
	ui_popup->child->setSize(w,h);
	ui_popup_close = func;
	ui_popup->show();
}

void ui_hidePopup()
{
	ui_popup->hide();
	if (ui_popup_close)
		ui_popup_close();

	ui_popup_close = NULL;
}

void ui_showCredits()
{
	ui_menu->hide();
	ui_popupImage("images/landscapes/credits.png",ui_showMenu);
}

void ui_setJump()
{
	char buf[255];
	ui_setter = INPUT_JUMP;
	sf::String txt = ui_controls[INPUT_JUMP]->getText();
	sprintf(buf,"-- %s --",static_cast<std::string>(txt).c_str());
	ui_controls[INPUT_JUMP]->setText(buf);
}

void ui_setLeft()
{
	char buf[255];
	ui_setter = INPUT_LEFT;
	sf::String txt = ui_controls[INPUT_LEFT]->getText();
	sprintf(buf,"-- %s --",static_cast<std::string>(txt).c_str());
	ui_controls[INPUT_LEFT]->setText(buf);
}

void ui_setRight()
{
	char buf[255];
	ui_setter = INPUT_RIGHT;
	sf::String txt = ui_controls[INPUT_RIGHT]->getText();
	sprintf(buf,"-- %s --",static_cast<std::string>(txt).c_str());
	ui_controls[INPUT_RIGHT]->setText(buf);
}

void ui_setCrouch()
{
	char buf[255];
	ui_setter = INPUT_CROUCH;
	sf::String txt = ui_controls[INPUT_CROUCH]->getText();
	sprintf(buf,"-- %s --",static_cast<std::string>(txt).c_str());
	ui_controls[INPUT_CROUCH]->setText(buf);
}

void ui_setShoot()
{
	char buf[255];
	ui_setter = INPUT_SHOOT;
	sf::String txt = ui_controls[INPUT_SHOOT]->getText();
	sprintf(buf,"-- %s --",static_cast<std::string>(txt).c_str());
	ui_controls[INPUT_SHOOT]->setText(buf);
}

void ui_setVol(double v)
{
	sf::Listener::SetGlobalVolume(v);
	char buf[255];
	sprintf(buf,"Volume: %.0f%%",v);
	ui_snd->setText(buf);
}

void ui_init()
{
	if (!Widget::fontUI.LoadFromFile("fonts/orbitron-bold.otf"))
		printf("failed to load font\n");

	initSfKeyToString();

	// create the actual ui items
	ui_base = new Widget(UI_CONTAINER,ui_base);
	ui_base->setBg("images/main_screen_background.png");

	ui_hud = new Widget(UI_CONTAINER,ui_base);
	ui_hud->toggleBg();
	ui_hud->hide();
	ui_menu = new Widget(UI_CONTAINER,ui_base);
	ui_menu->toggleBg();
	ui_menu->setPos(220,140);
	ui_pause = new Widget(UI_CONTAINER,ui_base);
	ui_pause->setPos(220,140);
	ui_pause->hide();
	ui_options = new Widget(UI_CONTAINER,ui_base);
	ui_options->toggleBg();
	ui_options->setPos(220,140);
	ui_options->hide();
	ui_opctrls = new Widget(UI_CONTAINER,ui_base);
	ui_opctrls->toggleBg();
	ui_opctrls->setPos(220,140);
	ui_opctrls->hide();

	ui_energy = new Widget(UI_PBAR,ui_hud);
	ui_energy->setPos(2,2);
	ui_lives = new Widget(UI_LABEL,ui_hud);
	ui_lives->setSize(200,20);
	ui_lives->setPos(10,30);

	ui_energyballs = new Widget(UI_LABEL,ui_hud);
	ui_energyballs->setSize(200,20);
	ui_energyballs->setPos(10,45);

	ui_popup = new Widget(UI_CONTAINER,ui_base);
	ui_popup->setPos(220,140);
	ui_popup->hide();

	// main menu

	Widget *b = new Widget(UI_BUTTON,ui_menu);
	b->setText("Start");
	b->setPos(62,60);
	b->setClick(ui_start);

	b = new Widget(UI_BUTTON,ui_menu);
	b->setText("Options");
	b->setPos(62,90);
	b->setClick(ui_showOptions);

	b = new Widget(UI_BUTTON,ui_menu);
	b->setText("Credits");
	b->setPos(62,120);
	b->setClick(ui_showCredits);

	b = new Widget(UI_BUTTON,ui_menu);
	b->setText("Quit");
	b->setPos(62,150);
	b->setClick(ui_quit);

	// popup
	b = new Widget(UI_BUTTON,ui_popup);
	b->toggleBg();
	b->setClick(ui_hidePopup);
	b->setAnyKey(ui_hidePopup);

	// pause menu
	b = new Widget(UI_LABEL,ui_pause);
	b->setText("Last Escape (Paused)");
	b->setPos(26,10);

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
	char buf[255];
	double svol = sf::Listener::GetGlobalVolume()*100.f;
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
	inputs[INPUT_JUMP].key = sf::Keyboard::Up;
	inputs[INPUT_LEFT].key = sf::Keyboard::Left;
	inputs[INPUT_RIGHT].key = sf::Keyboard::Right;
	inputs[INPUT_CROUCH].key = sf::Keyboard::Down;
	inputs[INPUT_SHOOT].key = sf::Keyboard::Space;
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
			Widget::ui_focused_widget = 0;
	}

	if (!r && (!ui_menu->isHidden() || !ui_pause->isHidden()))
		r = 1;

	return r;
}

void ui_render(Player * p)
{
	if(p) {
		Player & player = *p;
		char buf[256];
		double energy = 100*player.energy/player.energy_max;
		ui_energy->setSlideValue(energy);
		//sprintf(buf, "%.0f%%", energy);
		sprintf(buf, "%.0f/%.0f", player.energy, player.energy_max);
		ui_energy->setText(buf);

		sprintf(buf, "Lives: %d", player.lives);
		if (player.lives < 2)
			ui_lives->setTextColor(0xef, 0x29, 0x29);
		else
			ui_lives->setTextColor(0x01, 135, 0x00);
		ui_lives->setText(buf);


		sprintf(buf, "Energy Balls: %d", player.energyBalls);
		ui_energyballs->setTextColor(0x01, 135, 0x00);
		ui_energyballs->setText(buf);
	}

	ui_base->draw();
}

void ui_exit()
{
	delete ui_base;
}
