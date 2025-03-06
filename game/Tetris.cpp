#include "Tetris.h"
#include<iostream>

Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)
{
	this->rows = rows;
	this->cols = cols;
	this->left = left;
	this->top = top;
	this->blockSize = blockSize;

	for (int i = 0; i < rows; ++i) {
		vector<int>mapRow;
		for (int j = 0; j < cols; ++j) {
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}
	score = 0;
	removeLines = 0;
	level = 1;

	ifstream ifs(FILENAME);
	string Fline;
	if (ifs.is_open()) {
		while (getline(ifs, Fline)) {
			highestScore = stoi(Fline);
		}
	}
	else {
		cout << "¶ÁÈ¡Ê§°Ü" << endl;
		highestScore = 0;
	}
	ifs.close();
}

void Tetris::init()
{
	mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
	isGameOver = false;
	delay = NORMAL[0];

	srand(time(0));

	initgraph(938,896);

	loadimage(&background, "res/bg2.png");
	loadimage(&winUI, "res/win.png");
	loadimage(&overUI, "res/over.png");

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			map[i][j] = 0;
		}
	}
}

void Tetris::play()
{
	init();

	nowBlock = new Block;
	nextBlock = nowBlock;
	nowBlock = new Block;

	int time = 0;

	while (true) {

		keyEvent();

		time += getDelay();

		if (time > delay) {
			time = 0;
			drop();
			update = true;
		}
		
		if (update) {
			update = false;
			updateUI();
			clearLine();
		}

		if (isGameOver) {
			saveScore();
			showOver();
			system("pause");
			init();
		}
	}
}

void Tetris::move(int offset)
{
	lastBlock = *nowBlock;
	nowBlock->move(offset);
	if (!nowBlock->isOut(map)) {
		*nowBlock = lastBlock;
	}
}

void Tetris::rotate()
{
	if (nowBlock->getBlockType() == 6)return;
	lastBlock = *nowBlock;
	nowBlock->rotate();
	if (!nowBlock->isOut(map)) {
		*nowBlock = lastBlock;
	}
}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	setcolor(RGB(180, 180, 180));
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	outtextxy(670, 727, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", removeLines);
	gettextstyle(&f);
	int x = 224 - f.lfWidth * strlen(scoreText);
	outtextxy(x, 817, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	outtextxy(224 - 30, 727, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(670, 817, scoreText);
}

void Tetris::checkGameOver()
{
	isGameOver = (nowBlock->isOut(map) == false) ? true : false;
}

void Tetris::saveScore()
{
	if (score > highestScore) {
		highestScore = score;
		ofstream ofs(FILENAME);
		ofs << highestScore;
		ofs.close();
	}
}

void Tetris::showOver()
{
	mciSendString("stop res/bg.mp3", 0, 0, 0);
	if (level <= MAXLEVEL) {
		putimage(262, 361, &overUI);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else {
		putimage(262, 361, &winUI);
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}


int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

void Tetris::updateUI()
{
	putimage(0, 0, &background);

	IMAGE** imgs = Block::getImgs();

	BeginBatchDraw();
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (map[i][j] == 0)continue;
			int x = j * blockSize + left;
			int y = i * blockSize + top;
			putimage(x, y, imgs[map[i][j]]);
		}
	}

	nowBlock->draw(left, top);
	nextBlock->draw(545, 150);

	drawScore();
	EndBatchDraw();
}

void Tetris::drop()
{
	lastBlock = *nowBlock;
	nowBlock->drop();

	if (!nowBlock->isOut(map)) {
		lastBlock.stop(map);
		delete nowBlock;
		nowBlock = nextBlock;
		nextBlock = new Block;
		isStop = true;
		delay = NORMAL[level - 1];
		checkGameOver();
	}
}

void Tetris::clearLine()
{
	int line = 0;
	int saveLine = rows - 1;
	for (int i = rows - 1; i >= 0; --i) {
		int count = 0;
		for (int j = 0; j < cols; ++j) {
			if (map[i][j] != 0)count++;
			map[saveLine][j] = map[i][j];
		}
		line++;
		if (count < cols) {
			saveLine--;
			line--;
		}
	}


	if (line > 0) {
		int scoreLevel[4] = { 10,30,65,100 };
		score += scoreLevel[line -1];
		mciSendString("play res/xiaochu2.mp3", 0, 0, 0);
		level = (score + 99) / 100;
		removeLines += line;
		if (level > MAXLEVEL) {
			isGameOver = true;
		}
		update = true;
	}
}

void Tetris::keyEvent()
{
	unsigned char ch;
	bool isRotate = false;
	int dx = 0;
	if (_kbhit()) {
		ch = _getch();
		if (ch == 224) {
			ch = _getch();
			if (ch == 80) {
				delay = QUICK;
				isStop = false;
			}
			if (isStop) {
				switch (ch) {
				case 72:isRotate = true; break;
				case 75:dx = -1; break;
				case 77:dx = 1; break;
				default: break;
				}
			}
		}
	}

	if (isRotate) {
		isRotate = false;
		rotate();
	}

	if (dx != 0) {
		move(dx);
		update = true;
	}
}
