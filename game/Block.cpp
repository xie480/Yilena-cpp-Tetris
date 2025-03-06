#include "Block.h"
#include<iostream>

IMAGE* Block::imgs[8] = { nullptr, };
int Block::size = 36;

Block::Block()
{
	if (imgs[1] == nullptr) {
		IMAGE imgTmp;
		loadimage(&imgTmp, "res/tiles.png");

		SetWorkingImage(&imgTmp);

		for (int i = 1; i < 8; ++i) {
			imgs[i] = new IMAGE;
			getimage(imgs[i], i * size, 0, size, size);
		}

		SetWorkingImage();
	}

	/*
	这种构造方法避免了每个方块都得额外开辟一个二维数组出来，
	从而减少内存消耗
	*/
	int blocks[7][4] = {
		1,3,5,7,
		2,4,5,7,
		3,5,4,6,
		3,5,4,7,
		2,3,5,7,
		3,5,7,6,
		2,3,4,5
	};
	blockType = rand() % 7;

	for (int i = 0; i < 4; ++i) {
		int value = blocks[blockType][i];
		smallBlocks[i].row = value / 2;
		smallBlocks[i].col = value % 2 + 4;
	}

	img = imgs[blockType + 1];
}

void Block::drop()
{
	for (auto &blocks : smallBlocks) {
		blocks.row++;
	}
}

void Block::move(int offset)
{
	for (int i = 0; i < 4; ++i) {
		smallBlocks[i].col += offset;
	}
}

void Block::rotate()
{
	Point p = smallBlocks[1];
	for (int i = 0; i < 4; ++i) {
		//数学公式
		Point tmp = smallBlocks[i];
		smallBlocks[i].col = p.col - tmp.row + p.row;
		smallBlocks[i].row = p.row + tmp.col - p.col;
	}
}

void Block::draw(int leftMargin, int topMargin)
{
	for (int i = 0; i < 4; ++i) {
		int x = leftMargin + smallBlocks[i].col * size;
		int y = topMargin + smallBlocks[i].row * size;
		putimage(x, y,img);
	}
}

IMAGE** Block::getImgs()
{
	return imgs;
}

Block& Block::operator=(const Block& other)
{
	if (this == &other)return *this;

	this->blockType = other.blockType;

	for (int i = 0; i < 4; ++i) {
		this->smallBlocks[i] = other.smallBlocks[i];
	}

	return *this;
}

bool Block::isOut(const vector<vector<int>>& map)
{
	int rows = map.size();
	int cols = map[0].size();
	for (int i = 0; i < 4; ++i) {
		if (smallBlocks[i].col < 0 || smallBlocks[i].col >= cols ||
			smallBlocks[i].row < 0 || smallBlocks[i].row >= rows ||
			map[smallBlocks[i].row][smallBlocks[i].col]) {
			return false;
		}
	}
	return true;
}

void Block::stop(vector<vector<int>>& map)
{
	for (int i = 0; i < 4; ++i) {
		map[smallBlocks[i].row][smallBlocks[i].col] = blockType + 1;
	}
}

int Block::getBlockType()
{
	return blockType;
}