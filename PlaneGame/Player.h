
// Player.h : PlaneGame 飞机的一些特性
//
#pragma once

class Player
{
public:
	int p_x;	//-3 <= des_x <= 1451
	int p_y;	//0 <= des_y <= 685
	int p_width;
	int p_height;
	int p_blood;	//0 < blood <= 100
	int killenemy;
	int enemywin;

public:
	Player(){
		p_x = 480;
		p_y = 600;
		p_width = 137;
		p_height = 137;
		p_blood = 100;
		killenemy = 0;
		enemywin = 0;
	}
	void up();
	void down();
	void left();
	void right();
	void bloodmin(int x);
};