#include <graphics.h>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>
using namespace std;

int go = 1;
#define BLOCK_XY 25//С����߳�
/*��Ϸ����Ĵ�С*/
const int BLOCK_WIDTH = 10;//С����x������
const int BLOCK_HEIGHT = 20;//С����y������

#define WIDTH BLOCK_WIDTH * BLOCK_XY + BLOCK_XY * 6
#define HEIGHT BLOCK_HEIGHT * BLOCK_XY
int DOWNSPEND = 400;//���������ٶ�,��λ����
int SPEND = DOWNSPEND;
int score = 0;//����

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

/*��������*/
void display();
void clearscreen(COLORREF RGB);//��յ�ͼΪRGB��ɫ(��������)
void gamedrawpixal(int x, int y, COLORREF RGB);//��x��yλ�û���ɫΪRGB������
void drawfk(int k[], COLORREF RGB);//������˹����
void sand_fk();//�������һ������˹���鲢������root
void down_fk(int k[8], int fx);//�÷�����fxλ���ƶ�
void gameupdata();//��Ϸ����
int  gamepz(int k[8]);//��Ϸ��ײ��� 0������ײ��1����������ǽ�ڣ�2�������ײ�ǽ�ڣ�3��������������
void db();//�����ײ��Ĳ���
int  xc();//���صڼ��г��У����û�з���-1
void xh();//���в���
void xuanzhuan(int k[8]);//��תroot��ԭ��Ϊ�����е�һ��x��y
void gameover();//��Ϸ����
/*********/

void gameover() {
	while (1) {
		if (_kbhit()) {
			if (_getch() == 13) {
				break;
			}
		}
	};
	/*��ʼ��map����Ϊ0*/
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
	/*���map����Ļ*/
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
			/*����*/
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
	/*��������ײ�ǽ�� ��������������*/
	if (gamepz(root) == 2 || gamepz(root) == 3) {
		DOWNSPEND = SPEND;
		down_fk(root, UP);
		/*������˹�����λ��д��map����*/
		for (int i = 0; i < 4; i++) {
			if ((root[i * 2] < 0 || root[i * 2 + 1] < 0)) {
				gameover();
			}
			else {
				map[root[i * 2]][root[i * 2 + 1]] = 1;
			}
		}
		/*���map����Ļ*/
		for (int y = 0; y < BLOCK_HEIGHT; y++) {
			for (int x = 0; x < BLOCK_WIDTH; x++) {
				if (map[x][y] == 1) {
					gamedrawpixal(x, y, fk);
				}
			}
		}
		xh();//����
		sand_fk();//����һ������˹����
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
			/*�����������ǽ�ڻ������������ƻ�ȥ*/
			if (gamepz(root) == 3 || gamepz(root) == 1) {
				down_fk(root, RIGHT);
			}
			break;
		case RIGHT: 
			down_fk(root, RIGHT);
			/*�����������ǽ�ڻ������������ƻ�ȥ*/
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
			/*�����ת����ǽ�ڻ�����������ת��ȥ*/
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
	if (go) {//ֻ����һ��
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

	/*�������Ƶ��м�*/
	for (int i = 0; i < 4; i++){
		root[i * 2] = root[i * 2] + 5;
		root[i * 2 + 1] = root[i * 2 + 1] - 3;
	}
	/*��ʾ��һ������*/
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
	srand((unsigned)time(NULL));	//���������
	initgraph(WIDTH, HEIGHT);		//������ͼ����
	BeginBatchDraw();				//��ʼ������ͼ
	clearscreen(COLORREF RGB(255, 255, 255));
	DWORD t1, t2;
	t1 = t2 = GetTickCount();		//��ȡ���Կ���ʱ��
	
	/*��ʼ��map����Ϊ0*/
	for (int y = 0; y < BLOCK_HEIGHT; y++){
		for (int x = 0; x < BLOCK_WIDTH; x++){
			map[x][y] = 0;
		}
	}
	sand_fk();//����һ������˹����
	
	while (1) {

		/*ÿ��DOWNSPENDʱ������*/
		t1 = GetTickCount();
		if (t1 - t2 > DOWNSPEND) {
			down_fk(root, DOWN);
			db();
			t2 = t1;
		}
		/*��Ϸ����*/
		gameupdata();

		drawfk(root, fk);
		FlushBatchDraw();
		drawfk(root, COLORREF RGB(255, 255, 255));
		Sleep(1);
	}

	system("pause");
	return 0;
}