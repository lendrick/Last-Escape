#pragma once
#include "globals.h"

class Input {
public:
	Input();
	~Input();
	void poll();
	int direction();
	bool jump();
	bool quit();
	bool shoot();
	bool crouch();
	bool shooting();
	bool jumping();
	bool crouching();

private:
	void initFrame();

	int inputDirection;
	bool inputQuit;
	bool inputShoot;
	bool inputJump;
	bool inputCrouch;
};

typedef struct InputItem {
	sf::Key::Code key;
	sf::String text;
} InputItem;

#define INPUT_JUMP 0
#define INPUT_LEFT 1
#define INPUT_RIGHT 2
#define INPUT_CROUCH 3
#define INPUT_SHOOT 4

extern InputItem inputs[5];
