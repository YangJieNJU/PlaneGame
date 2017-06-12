
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "PlaneGame.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//定时器的名称用宏比较清楚  
#define TIMER_PAINT 1  
#define TIMER_HEROMOVE 2  

// CChildView

CChildView::CChildView()
{
	type = 0;
	//设置地图初始从最左端开始显示  
	m_yMapEnd = 0;
	time_background = 0;
	num_bullet = 0;
	time_enemy = 0;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	//加载背景  
	m_background.Load(L"Background.jpg");

	//加载飞机图片  
	m_plane.Load(L"Plane.png");
	//TransparentPNG(&m_plane);

	//加载子弹图片
	m_bullet.Load(L"Bullet.png");

	//加载敌人图片
	m_enemy.Load(L"Enemy.png");

	//加载敌人子弹图片
	m_ebullet.Load(L"Bullet2.png");

	return TRUE;
}

void CChildView::OnPaint() 
{	
	//计时系统
	time_background++;
	time_enemy++;

	// TODO:  在此处添加消息处理程序代码
	CDC *cDC = this->GetDC();   //获得当前窗口的DC     
	GetClientRect(&m_client);   //获得窗口的尺寸  
	//创建缓冲DC  
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);

	//画背景
	if (m_yMapEnd < 7153)
		//m_background.Draw(m_cacheDC, 0, 100, 1024, 900, 0, 0, 1024, 900);
		m_background.Draw(m_cacheDC, 0, 0, 1024, 900, 0, 7153 - m_yMapEnd, 1024, 900);
	else{
		m_background.Draw(m_cacheDC, 0, 0, 1024, m_yMapEnd - 7153, 0, 15206 - m_yMapEnd, 1024, m_yMapEnd - 7153);
		m_background.Draw(m_cacheDC, 0, m_yMapEnd - 7153, 1024, 8053 - m_yMapEnd, 0, 0, 1024, 8053 - m_yMapEnd);
	}

	if (type == 1){
		//画飞机 
		m_planePos.left = player.p_x;
		m_planePos.right = player.p_x + player.p_width;
		m_planePos.top = player.p_y;
		m_planePos.bottom = player.p_y + player.p_height;
		m_plane.Draw(m_cacheDC, m_planePos);

		//画子弹
		for (vector<MyBullet>::iterator t = MyBullets.begin(); t != MyBullets.end();){
			m_bulletPos.left = t->b_x;
			m_bulletPos.right = t->b_x + t->b_width;
			m_bulletPos.top = t->b_y;
			m_bulletPos.bottom = t->b_y + t->b_height;
			m_bullet.Draw(m_cacheDC, m_bulletPos);

			++t;
		}

		//画敌人
		for (vector<Enemy>::iterator t = Enemys.begin(); t != Enemys.end();){
			m_enemyPos.left = t->e_x;
			m_enemyPos.right = t->e_x + t->e_width;
			m_enemyPos.top = t->e_y;
			m_enemyPos.bottom = t->e_y + t->e_height;
			m_enemy.Draw(m_cacheDC, m_enemyPos);

			++t;
		}

		//画敌人子弹
		for (vector<EnemyBullet>::iterator t = EnemyBullets.begin(); t != EnemyBullets.end();){
			m_ebulletPos.left = t->b_x;
			m_ebulletPos.right = t->b_x + t->b_width;
			m_ebulletPos.top = t->b_y;
			m_ebulletPos.bottom = t->b_y + t->b_height;
			m_ebullet.Draw(m_cacheDC, m_ebulletPos);

			++t;
		}

		//写字
		//CDC *pDC = GetDC();
		m_cacheDC.SetBkMode(TRANSPARENT);
		m_cacheDC.SetTextColor(RGB(255, 255, 255));
		CString first("Plane War");
		m_cacheDC.TextOutW(20, 50, first);
		CString a("玩家生命值：");
		CString b;
		b.Format(_T("%d"), player.p_blood);
		CString second = a + b;
		m_cacheDC.TextOutW(20, 80, second);
		CString c("消灭敌人数目：");
		CString d;
		d.Format(_T("%d"), player.killenemy);
		CString thrid = c + d;
		m_cacheDC.TextOutW(20, 110, thrid);
		CString e("敌人突破防线数目：");
		CString f;
		f.Format(_T("%d"), player.enemywin);
		CString forth = e + f;
		m_cacheDC.TextOutW(20, 140, forth);

		//子弹更新逻辑
		for (vector<MyBullet>::iterator t = MyBullets.begin(); t != MyBullets.end();){
			t->MoveUp();
			int y = t->b_y;
			if (y < 1){
				t = MyBullets.erase(t);
			}
			else
				++t;
		}

		//敌人及敌人子弹更新逻辑
		for (vector<EnemyBullet>::iterator t = EnemyBullets.begin(); t != EnemyBullets.end();){
			t->MoveDown();
			int y = t->b_y;
			if (y < 1){
				t = EnemyBullets.erase(t);
			}
			else
				++t;
		}
		if (time_enemy % 10 == 0){
			for (vector<Enemy>::iterator t1 = Enemys.begin(); t1 != Enemys.end();){
				if (time_enemy == 100){
					int ifbullet = rand() % 3;
					if (ifbullet == 0){
						EnemyBullet b(t1->e_x + t1->e_width / 2 - 20, t1->e_y + 40);
						EnemyBullets.push_back(b);
					}
				}
				if (time_enemy == 100){
					int nextmove = rand() % 3 + 1;
					if (nextmove == 1)
						t1->ToLeft();
					else if (nextmove == 2)
						t1->ToRight();
					else if (nextmove > 2)
						t1->ToDown();
				}
				else
					t1->ToDown();
				t1++;

			}
			if (time_enemy == 100){
				int enemy_x = rand() % (1000) - 30;
				int enemy_y = 40;
				Enemy e(enemy_x, enemy_y);
				Enemys.push_back(e);
				time_enemy = 0;
			}
		}


		//如果玩家子弹打到敌方子弹
		for (vector<MyBullet>::iterator t = MyBullets.begin(); t != MyBullets.end();){
			int flag = 0;
			for (vector<EnemyBullet>::iterator t1 = EnemyBullets.begin(); t1 != EnemyBullets.end();){
				if ((abs(t1->b_x - t->b_x) <= t1->b_width / 2) && (abs(t1->b_y - t->b_y) <= t1->b_height)){
					flag = 1;
					t1 = EnemyBullets.erase(t1);
					break;
				}
				else
					++t1;
			}
			if (flag == 1)
				t = MyBullets.erase(t);
			else
				++t;
		}

		//如果玩家子弹打到敌人
		for (vector<Enemy>::iterator t = Enemys.begin(); t != Enemys.end();){
			int flag = 0;
			if (t->e_y >= 860){
				flag = 1;
				player.bloodmin(10);
				player.enemywin++;
			}
			else{
				for (vector<MyBullet>::iterator t1 = MyBullets.begin(); t1 != MyBullets.end();){
					if ((abs(t1->b_x - t->e_x) <= t1->b_width) && (abs(t1->b_y - t->e_height) <= t->e_height)){
						flag = 1;
						t1 = MyBullets.erase(t1);
						player.killenemy++;
						player.bloodmin(-10);

						CRect m_boomPos;    //保存特效的位置  
						CImage m_boom;   //特效
						m_boom.Load(L"boom.png");
						m_boomPos.left = t->e_x;
						m_boomPos.right = t->e_x + t->e_width;
						m_boomPos.top = t->e_y;
						m_boomPos.bottom = t->e_y + t->e_height;
						m_boom.Draw(m_cacheDC, m_boomPos);
						break;
					}
					else
						++t1;
				}
			}
			if (flag == 1)
				t = Enemys.erase(t);
			else
				++t;
		}

		//如果敌人子弹打到我
		for (vector<EnemyBullet>::iterator t1 = EnemyBullets.begin(); t1 != EnemyBullets.end();){
			if ((abs(t1->b_x - player.p_x) <= t1->b_width) && (abs(t1->b_y - player.p_y) <= t1->b_height)){
				t1 = EnemyBullets.erase(t1);
				player.bloodmin(10);
				break;
			}
			else
				++t1;
		}

		//如果敌机撞到我
		for (vector<Enemy>::iterator t1 = Enemys.begin(); t1 != Enemys.end();){
			if ((abs(t1->e_x - player.p_x) <= t1->e_width) && (abs(t1->e_y - player.p_y) <= t1->e_height)){
				t1 = Enemys.erase(t1);
				player.bloodmin(30);
				player.killenemy++;
				break;
			}
			else
				++t1;
		}
	}
	
	//背景更新逻辑
	if (time_background == 5){
		if (m_yMapEnd < 8053)
			m_yMapEnd += 10;
		else
			m_yMapEnd = 0;
		time_background = 0;
	}

	if (player.p_blood <= 0)
		type = 3;

	if (type == 0){
		CRect m_startPos;    //保存特效的位置  
		CImage m_start;   //特效
		m_start.Load(L"start.png");
		m_startPos.left = 250;
		m_startPos.right = 250 + 440;
		m_startPos.top = 200;
		m_startPos.bottom = 200 + 106;
		m_start.Draw(m_cacheDC, m_startPos);

		m_cacheDC.SetBkMode(TRANSPARENT);
		m_cacheDC.SetTextColor(RGB(255, 255, 255));
		CString first("W S A D 控制上下左右移动，K 发射子弹");
		m_cacheDC.TextOutW(330, 330, first);
		CString second("按 K 进入游戏");
		m_cacheDC.TextOutW(440, 370, second);

		CString third("捐赠开发者");
		m_cacheDC.TextOutW(450, 420, third);
		CRect m_donPos;    //保存特效的位置  
		CImage m_don;   //特效
		m_don.Load(L"donate.jpg");
		m_donPos.left = 350;
		m_donPos.right = 350 + 300;
		m_donPos.top = 450;
		m_donPos.bottom = 450 + 300;
		m_don.Draw(m_cacheDC, m_donPos);
	}

	if (type == 3){
		m_cacheDC.SetBkMode(TRANSPARENT);
		m_cacheDC.SetTextColor(RGB(255, 255, 255));
		CString first("Plane War");
		m_cacheDC.TextOutW(20, 50, first);
		CString a("玩家生命值：");
		CString b;
		b.Format(_T("%d"), player.p_blood);
		CString second = a + b;
		m_cacheDC.TextOutW(20, 80, second);
		CString c("消灭敌人数目：");
		CString d;
		d.Format(_T("%d"), player.killenemy);
		CString thrid = c + d;
		m_cacheDC.TextOutW(20, 110, thrid);
		CString e("敌人突破防线数目：");
		CString f;
		f.Format(_T("%d"), player.enemywin);
		CString forth = e + f;
		m_cacheDC.TextOutW(20, 140, forth);
		CString fifth("按 K 重新开始游戏");
		m_cacheDC.TextOutW(20, 170, fifth);
		CRect m_overPos;    //保存特效的位置  
		CImage m_over;   //特效
		m_over.Load(L"gameover.png");
		m_overPos.left = 250;
		m_overPos.right = 250 + 515;
		m_overPos.top = 400;
		m_overPos.bottom = 400 + 99;
		m_over.Draw(m_cacheDC, m_overPos);
	}



	cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);
	//在绘制完图后,使窗口区有效  
	ValidateRect(&m_client);
	//释放缓冲DC  
	m_cacheDC.DeleteDC();
	//释放对象  
	m_cacheCBitmap.DeleteObject();
	//释放窗口DC  
	ReleaseDC(cDC);
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
}

//按键响应函数  
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//nChar表示按下的键值  
	switch (nChar)
	{
	case 'd': 
	case 'D':
		if (m_planePos.right < 1035){
			m_planePos.left += 10;    //向右移动10个像素的单位  
			m_planePos.right += 10;   //左边和右边都要移动哦
			player.right();
		}
		break;
	case 'a':
	case 'A':
		if (m_planePos.left > -30){
			m_planePos.left -= 10;
			m_planePos.right -= 10;
			player.left();
		}
		break;
	case 'w':
	case 'W':
		if (m_planePos.top > 40){
			m_planePos.top -= 10;
			m_planePos.bottom -= 10;
			player.up();
		}
		break;
	case 's':
	case 'S':
		if (m_planePos.bottom < 865){
			m_planePos.top += 10;
			m_planePos.bottom += 10;
			player.down();
		}
		break;
	case 'k':
	case 'K':
		if (type == 1){
			num_bullet++;
			MyBullet b((m_planePos.left + m_planePos.right) / 2 - 20, m_planePos.top - 40);
			MyBullets.push_back(b);
		}
		else if (type == 0){
			type = 1;
			num_bullet = 0;
			time_enemy = 0;
		}
		else if (type == 3){
			num_bullet = 0;
			time_enemy = 0;
			type = 1;
			player.p_x = 480;
			player.p_y = 600;
			player.p_width = 137;
			player.p_height = 137;
			player.p_blood = 100;
			player.killenemy = 0;
			player.enemywin = 0;
			MyBullets.clear();
			Enemys.clear();
			EnemyBullets.clear();
		}
		break;
	}
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	//CWnd::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	case TIMER_PAINT:OnPaint(); break;  //若是重绘定时器，就执行OnPaint函数  
	case TIMER_HEROMOVE:               //控制人物移动的定时器  
		m_planePos.left += 10;
		m_planePos.right += 10;
		break;
	}
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码  

	//创建一个10毫秒产生一次消息的定时器  
	SetTimer(TIMER_PAINT, 10, NULL);

	return 0;
}
