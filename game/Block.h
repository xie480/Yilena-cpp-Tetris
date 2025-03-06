#pragma once
#include<graphics.h>
#include<vector>
using namespace std;

struct Point {
	int col;
	int row;
};

class Block {
public:
	Block();
	void drop();
	void move(int offset);
	void rotate();
	void draw(int leftMargin, int topMargin);
	static IMAGE** getImgs();
	Block& operator=(const Block& other);
	bool isOut(const vector<vector<int>>& map);
	void stop(vector<vector<int>>& map);
	int getBlockType();

private:
	int blockType;
	Point smallBlocks[4];
	IMAGE* img;

	static IMAGE* imgs[8];
	static int size;
};
