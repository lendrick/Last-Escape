#pragma once

class Input {
public:
	Input();
	~Input();
	void poll();
	int direction();
	bool jump();
	bool quit();
	bool shoot();
	
private:
	void initFrame();
	
	int inputDirection;
	bool inputQuit;
	bool inputShoot;
	bool inputJump;
};
