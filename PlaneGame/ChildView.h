
// ChildView.h : CChildView 类的接口
//

#pragma once
#include "Player.h"
#include "MyBullet.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include <string>
#include<vector>
#include<math.h>
#include <iomanip>
using namespace std;

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	int type;//0进入菜单，1开始游戏，2暂停，3结束

	CDC m_cacheDC;   //缓冲DC  
	CBitmap m_cacheCBitmap;//缓冲位图
	CRect m_client;    //保存客户区大小  
	CImage m_background;      //背景图片 
	int m_yMapEnd;     //y方向上地图的终点  
	int time_background;

	Player player;
	CRect m_planePos;    //保存飞机的位置  
	CImage m_plane;   //飞机

	vector <MyBullet> MyBullets;
	int num_bullet;
	CRect m_bulletPos;    //保存子弹的位置  
	CImage m_bullet;   //子弹

	vector<Enemy> Enemys;
	CRect m_enemyPos;    //保存敌人的位置  
	CImage m_enemy;   //敌人
	int time_enemy;

	vector <EnemyBullet> EnemyBullets;
	CRect m_ebulletPos;    //保存子弹的位置  
	CImage m_ebullet;   //子弹

// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

