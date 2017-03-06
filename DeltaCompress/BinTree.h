
# pragma once

#include <vector>

struct binElem
{
	char init;
	char val;
	unsigned char lvl;
	unsigned int cnt;
	unsigned int zero;
	unsigned int one;
	unsigned int up;
};

struct binHead32
{
	unsigned int len;
	unsigned int val;
	unsigned int cnt;
};

struct binHead16
{
	unsigned int len;
	unsigned short val;
	unsigned int cnt;
};

using namespace std;

class BinTree
{
public:
	BinTree() {};
	~BinTree() {};
	
	vector<binElem> binData;

	unsigned int initilizeBinData(const unsigned int& bits);
	unsigned int addItemSpanning(const unsigned int &item);
	unsigned int addItem(const unsigned int &item);
	unsigned int addItem(const unsigned short &item);
	string ToString(const unsigned int &val, unsigned int mask = 0x80000000);
	string ToString(const unsigned short &val, unsigned short mask = 0x8000);

private:
	unsigned int depth;
	unsigned int treeSize;

	void expandTree();
	void initElement(const int &val, const unsigned int &up);
	unsigned int addInSpanTree(const unsigned int &item, unsigned int pos, const unsigned int &mask);
	unsigned int addInTree(const unsigned int &item, const unsigned int &pos, const unsigned int &mask);
	unsigned int addInTree(const unsigned short &item, const unsigned int &pos, const unsigned short &mask);
};
