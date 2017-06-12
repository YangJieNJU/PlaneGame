
// EnemyBullet.h : 敌人的子弹的一些特性
//
#pragma once

class EnemyBullet{
public:
	int b_x;
	int b_y;
	int b_width;
	int b_height;

public:
	EnemyBullet(int x, int y){
		b_x = x;
		b_y = y - 10;
		b_width = 50;
		b_height = 50;
	}
	void MoveDown();
};
