
// Enemy.h : 我的子弹的一些特性
//
#pragma once

class Enemy{
public:
	int e_x;	
	int e_y;	
	int e_width;
	int e_height;
public:
	Enemy(int x, int y){
		e_x = x;
		e_y = y;
		e_width = 90;
		e_height = 90;
	}
	void ToLeft();
	void ToRight();
	void ToDown();
	
};