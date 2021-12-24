#include <graphics.h>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>
using namespace std;

int go = 1;
#define BLOCK_XY 25//小方格边长
/*游戏区域的大小*/
const int BLOCK_WIDTH = 10;//小方格x轴数量
const int BLOCK_HEIGHT = 20;//小方格y轴数量

#define WIDTH BLOCK_WIDTH * BLOCK_XY + BLOCK_XY * 6
#define HEIGHT BLOCK_HEIGHT * BLOCK_XY
int DOWNSPEND = 400;//方块下落速度,单位毫秒
int SPEND = DOWNSPEND;
int score = 0;//分数

//up 119  down 115  left 97  right 100  \n 13  " " 32  e 101
#define UP 119
#define DOWN 115
#define LEFT 97
#define RIGHT 100

#define fk COLORREF RGB(255, 0, 0)

int map[BLOCK_WIDTH][BLOCK_HEIGHT] = { 0 };
int root[8] = { 0 };
int root2[8] = { 0 };
//I
int I[8] = { 0, 1, 0, 0, 0, 2, 0, 3 };
//L
int L[8] = { 0, 3, 0, 2, 1, 3, 2, 3 };
//J
int J[8] = { 2, 3, 2, 2, 1, 3, 0, 3 };
//Z
int Z[8] = { 1, 3, 1, 2, 0, 2, 2, 3 };
//N
int N[8] = { 1, 3, 1, 2, 2, 2, 0, 3 };
//O
int O[8] = { 0, 2, 0, 3, 1, 2, 1, 3 };
//T
int T[8] = { 1, 3, 1, 2, 0, 3, 2, 3 };

/*函数声明*/
void display();
void clearscreen(COLORREF RGB);//清空地图为RGB颜色(带有网格)
void gamedrawpixal(int x, int y, COLORREF RGB);//在x，y位置画颜色为RGB的网格
void drawfk(int k[], COLORREF RGB);//画俄罗斯方块
void sand_fk();//随机生成一个俄罗斯方块并拷贝到root
void down_fk(int k[8], int fx);//让方块向fx位置移动
void gameupdata();//游戏更新
int  gamepz(int k[8]);//游戏碰撞检测 0：无碰撞，1：碰到左右墙壁，2：碰到底部墙壁，3：碰到其它方块
void db();//碰到底部的操作
int  xc();//返回第几行成行，如果没有返回-1
void xh();//消行操作
void xuanzhuan(int k[8]);//旋转root，原点为数组中第一组x，y
void gameover();//游戏结束
/*********/

void gameover() {
	while (1) {
		if (_kbhit()) {
			if (_getch() == 13) {
				break;
			}
		}
	};
	/*初始化map数组为0*/
	for (int y = 0; y < BLOCK_HEIGHT; y++) {
		for (int x = 0; x < BLOCK_WIDTH; x++) {
			map[x][y] = 0;
		}
		display();
		Sleep(15);
	}
	//go = 1;
}
void xuanzhuan(int k[8]) {
	int temp[8];
	int temp2[8];
	memcpy(temp2, k, sizeof(int) * 8);
	temp2[0] = 0;
	temp2[1] = 0;
	for (int i = 1; i < 4; i++){
		temp2[i * 2] = temp2[i * 2] - k[0];
		temp2[i * 2 + 1] = temp2[i * 2 + 1] - k[1];
	}
	for (int i = 0; i < 4; i++){
		temp[i * 2] = -temp2[i * 2 + 1];
		temp[i * 2 + 1] = temp2[i * 2];
	}
	for (int i = 0; i < 4; i++){
		temp[i * 2] = temp[i * 2] + k[0];
		temp[i * 2 + 1] = temp[i * 2 + 1] + k[1];
	}

	memcpy(k, temp, sizeof(int) * 8);
}
void display() {
	/*输出map到屏幕*/
	for (int y = 0; y < BLOCK_HEIGHT; y++) {
		for (int x = 0; x < BLOCK_WIDTH; x++) {
			if (map[x][y] == 1) {
				gamedrawpixal(x, y, fk);
			}
			if (map[x][y] == 0) {
				gamedrawpixal(x, y, COLORREF RGB(255, 255, 255));
			}
		}
	}
	FlushBatchDraw();
}
void xh() {
	while (xc() != -1) {
		int m = xc();
			for (int x = 0; x < BLOCK_WIDTH; x++) {
				map[x][m] = 0;
			}
			display();
			Sleep(200);
			/*下移*/
			for (int y = m; y > 0; y--){
				for (int x = 0; x < BLOCK_WIDTH; x++){
					map[x][y] = map[x][y - 1];
				}
			}
			display();
			Sleep(200);
		}
}
int xc() {
	int temp = 0;
	for (int y = 0; y < BLOCK_HEIGHT; y++) {
		for (int x = 0; x < BLOCK_WIDTH; x++) {
			if (map[x][y] == 1) {
				++temp;
			}
			if (temp >= BLOCK_WIDTH) return y;
		}
		temp = 0;
	}
	return -1;
}
void db() {
	/*如果碰到底部墙壁 或碰到其它方块*/
	if (gamepz(root) == 2 || gamepz(root) == 3) {
		DOWNSPEND = SPEND;
		down_fk(root, UP);
		/*将俄罗斯方块的位置写入map数组*/
		for (int i = 0; i < 4; i++) {
			if ((root[i * 2] < 0 || root[i * 2 + 1] < 0)) {
				gameover();
			}
			else {
				map[root[i * 2]][root[i * 2 + 1]] = 1;
			}
		}
		/*输出map到屏幕*/
		for (int y = 0; y < BLOCK_HEIGHT; y++) {
			for (int x = 0; x < BLOCK_WIDTH; x++) {
				if (map[x][y] == 1) {
					gamedrawpixal(x, y, fk);
				}
			}
		}
		xh();//消行
		sand_fk();//生成一个俄罗斯方块
	}
}
int gamepz(int k[8]) {
	int temp = 0;
	for (int i = 0; i < 4; i++){
		if (k[i * 2] < 0 || k[i * 2] > BLOCK_WIDTH - 1) temp = 1;
		if (k[i * 2 + 1] > BLOCK_HEIGHT - 1) temp = 2;
		if (!(root[i * 2] < 0 || root[i * 2 + 1] < 0))
		if (map[root[i * 2]][root[i * 2 + 1]] == 1) temp = 3;
	}
	return temp;
}
void gameupdata() {
	char ch;
	if (_kbhit()) {
		ch = _getch();
		switch (ch)
		{
		case LEFT: 
			down_fk(root, LEFT);
			/*如果左移碰到墙壁或其他方块则移回去*/
			if (gamepz(root) == 3 || gamepz(root) == 1) {
				down_fk(root, RIGHT);
			}
			break;
		case RIGHT: 
			down_fk(root, RIGHT);
			/*如果右移碰到墙壁或其他方块则移回去*/
			if (gamepz(root) == 3 || gamepz(root) == 1) {
				down_fk(root, LEFT);
			}
			break;
		case DOWN: 
			down_fk(root, DOWN);
			db();
			DOWNSPEND = 10;
			break;
		case 32:
			xuanzhuan(root);
			/*如果旋转碰到墙壁或其他方块则转回去*/
			if (gamepz(root) == 3 || gamepz(root) == 1 || gamepz(root) == 2) {
				xuanzhuan(root);
				xuanzhuan(root);
				xuanzhuan(root);
			}
			break;
		default:
			break;
		}
	}
	
}
void down_fk(int k[8], int fx) {
	switch (fx)
	{
	case UP:
		for (int i = 0; i < 4; i++) {
			k[i * 2 + 1] = k[i * 2 + 1] - 1;
		}
		break;
	case DOWN: 
		for (int i = 0; i < 4; i++) {
			k[i * 2 + 1] = k[i * 2 + 1] + 1;
		}
		break;
	case LEFT: 
		for (int i = 0; i < 4; i++) {
			k[i * 2] = k[i * 2] - 1;
		}
		break;
	case RIGHT: 
		for (int i = 0; i < 4; i++) {
			k[i * 2] = k[i * 2] + 1;
		}
		break;
	default: break;
	}
}
void sand_fk() {
	memcpy(root, root2, sizeof(int) * 8);
	if (go) {//只运行一次
		go = 0;
		int s = rand() % 7;
		switch (s)
		{
		case 0: memcpy(root, I, sizeof(int) * 8); break;
		case 1: memcpy(root, L, sizeof(int) * 8); break;
		case 2: memcpy(root, J, sizeof(int) * 8); break;
		case 3: memcpy(root, Z, sizeof(int) * 8); break;
		case 4: memcpy(root, N, sizeof(int) * 8); break;
		case 5: memcpy(root, O, sizeof(int) * 8); break;
		case 6: memcpy(root, T, sizeof(int) * 8); break;
		default: break;
		}
	}
	int ss = rand() % 7;
	switch (ss)
	{
		case 0: memcpy(root2, I, sizeof(int) * 8); break;
		case 1: memcpy(root2, L, sizeof(int) * 8); break;
		case 2: memcpy(root2, J, sizeof(int) * 8); break;
		case 3: memcpy(root2, Z, sizeof(int) * 8); break;
		case 4: memcpy(root2, N, sizeof(int) * 8); break;
		case 5: memcpy(root2, O, sizeof(int) * 8); break;
		case 6: memcpy(root2, T, sizeof(int) * 8); break;
		default: break;
	}

	/*将方块移到中间*/
	for (int i = 0; i < 4; i++){
		root[i * 2] = root[i * 2] + 5;
		root[i * 2 + 1] = root[i * 2 + 1] - 3;
	}
	/*显示下一个方块*/
	for (int i = 0; i < 4; i++) {
		gamedrawpixal(BLOCK_WIDTH + 1, i, COLORREF RGB(255, 255, 255));
		gamedrawpixal(BLOCK_WIDTH + 2, i, COLORREF RGB(255, 255, 255));
		gamedrawpixal(BLOCK_WIDTH + 3, i, COLORREF RGB(255, 255, 255));
		gamedrawpixal(BLOCK_WIDTH + 4, i, COLORREF RGB(255, 255, 255));
	}
	for (int i = 0; i < 4; i++){
		gamedrawpixal(root2[i * 2] + BLOCK_WIDTH + 1, root2[i * 2 + 1], fk);
	}

}
void drawfk(int k[], COLORREF RGB) {
	for (int i = 0; i < 4; i++){
		gamedrawpixal(k[i * 2], k[i * 2 + 1], RGB);
	}
}
void clearscreen(COLORREF RGB) {
	for (int y = 0; y < BLOCK_HEIGHT; y++) {
		for (int x = 0; x < BLOCK_WIDTH; x++) {
			gamedrawpixal(x, y, RGB);
		}
	}
}
void gamedrawpixal(int x, int y, COLORREF RGB) {
	setfillcolor(RGB); setlinecolor(COLORREF RGB(0, 0, 0));
	fillrectangle(x * BLOCK_XY, y * BLOCK_XY, BLOCK_XY + x * BLOCK_XY, BLOCK_XY + y * BLOCK_XY);
}
int main() {
	srand((unsigned)time(NULL));	//随机数种子
	initgraph(WIDTH, HEIGHT);		//创建绘图窗口
	BeginBatchDraw();				//开始批量绘图
	clearscreen(COLORREF RGB(255, 255, 255));
	DWORD t1, t2;
	t1 = t2 = GetTickCount();		//获取电脑开机时间
	
	/*初始化map数组为0*/
	for (int y = 0; y < BLOCK_HEIGHT; y++){
		for (int x = 0; x < BLOCK_WIDTH; x++){
			map[x][y] = 0;
		}
	}
	sand_fk();//生成一个俄罗斯方块
	
	while (1) {

		/*每隔DOWNSPEND时间下落*/
		t1 = GetTickCount();
		if (t1 - t2 > DOWNSPEND) {
			down_fk(root, DOWN);
			db();
			t2 = t1;
		}
		/*游戏更新*/
		gameupdata();
		/*画俄罗斯方块*/
		drawfk(root, fk);
		FlushBatchDraw();
		drawfk(root, COLORREF RGB(255, 255, 255));
		Sleep(1);
	}

	system("pause");
	return 0;
}
