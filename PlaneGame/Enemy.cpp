
// Enemy.cpp : Enemy 类的实现
//

#include "stdafx.h"
#include "Enemy.h"

void Enemy::ToLeft(){
	if (e_x >= -30)
		e_x -= 10;
}

void Enemy::ToRight(){
	if (e_x <= 1035)
		e_x += 10;
}

void Enemy::ToDown(){
	if (e_y <= 865)
		e_y += 10;
}