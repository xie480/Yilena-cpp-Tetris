#pragma once
#include<vector>
#include<graphics.h>
#include "Block.h"
#include <cstdlib>
#include<ctime>
#include<conio.h>
#include<mmstream.h>
#include<string>
#include<fstream>
#pragma comment(lib,"winmm.lib")
using namespace std;

#define MAXLEVEL 5
#define FILENAME "save.txt"

class Tetris {
public:
	Tetris(int cols, int rows, int left, int top, int blockSize);
	void init();
	void play();

private:
	int delay;
	bool update;
	vector<vector<int>>map;
	int rows;
	int cols;
	int left;
	int top;
	int blockSize;
	IMAGE background;
	Block* nowBlock;
	Block* nextBlock;
	Block lastBlock;
	bool isStop = true;
	int score = 0;
	int level;
	int removeLines;
	int highestScore;
	bool isGameOver = false;
	IMAGE overUI;
	IMAGE winUI;

	const int NORMAL[MAXLEVEL] = { 500,300,200,125,90 };
	const int QUICK = 50;

	int getDelay();
	void updateUI();
	void drop();
	void clearLine();
	void keyEvent();
	void move(int offset);
	void rotate();
	void drawScore();
	void checkGameOver();
	void saveScore();
	void showOver();
};