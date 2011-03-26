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
	bool shooting();
	bool jumping();
	
private:
	void initFrame();
	
	int inputDirection;
	bool inputQuit;
	bool inputShoot;
	bool inputJump;
};
