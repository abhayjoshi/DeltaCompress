# include "BinTree.h"
# include <iostream>
# include <string>
# include <sstream>

//vector<binElem> binData;
//unsigned int depth;


unsigned int BinTree::initilizeBinData(const unsigned int &bits)
{
	size_t max_len = ((size_t)(-1) / sizeof (binElem));
	size_t tmp_len = max_len;
	size_t max_bits = 0;
	size_t siz = sizeof (size_t);
	bool falseBit = false;
	
	tmp_len >>= 1;

	while (tmp_len)
	{
		if (!falseBit && ((tmp_len & 1) == 0)) 
			falseBit = true;
		tmp_len >>= 1;
		max_bits++;
	}

	if (!falseBit)
		max_bits++;

	depth = std::min(bits, --max_bits);
	unsigned int sz = (1 << (depth + 1)) - 2;
	unsigned int sz2 = (sz + 1) / 2 - 1;
	vector<binElem>::size_type len = binData.max_size();
	size_t l1 = sizeof (binElem);
	size_t l2 = ((size_t)(-1));
	size_t l = ((size_t)(-1) / sizeof (binElem));

	try
	{
		binData.resize(sz);
	}
	catch (exception ex)
	{
		return 1;
	}

	unsigned int i = 0, npos = 2, lvl = 0;

	for (; i < sz2; i++)
	{
		if ((pow(2, lvl + 2) - 2) <= i)
			lvl++;

		binData[i].val = -1;
		binData[i].cnt = 0;
		binData[i].lvl = lvl;
		binData[i].zero = npos;
		binData[npos].up = i;
		npos++;
		binData[i].one = npos;
		binData[npos].up = i;
		npos++;
		i++;
		binData[i].val = 1;
		binData[i].cnt = 0;
		binData[i].lvl = lvl;
		binData[i].zero = npos;
		binData[npos].up = i;
		npos++;
		binData[i].one = npos;
		binData[npos].up = i;
		npos++;
	}
	
	lvl++;

	for (; i < sz; i++)
	{
		binData[i].val = -1;
		binData[i].cnt = 0;
		binData[i].lvl = lvl;
		binData[i].zero = 0;
		binData[i].one = 0;
		i++;
		binData[i].val = 1;
		binData[i].cnt = 0;
		binData[i].lvl = lvl;
		binData[i].zero = 0;
		binData[i].one = 0;
	}

	return 0;
}

string BinTree::ToString(const unsigned int &val, unsigned int mask)
{
	unsigned int i = 0, msk = 0x80000000;
	string out;
	out.resize(32,' ');

	while (msk != mask)
	{
		msk >>= 1;
		i++;
	}

	while (mask)
	{
		out[i++] = (val & mask ? '1' : '0');
		mask >>= 1;
	}

	return out;
}

void BinTree::expandTree()
{
	size_t i = binData.size();
	binData.resize(binData.size() + 1000);

	for (; i < binData.size(); ++i)
		binData[i].init = 1;
}

void BinTree::initElement(const int &val, const unsigned int &up)
{
	if (binData.size() <= treeSize)
		expandTree();

	if (binData[treeSize].init == 1)
	{
		binData[treeSize].val = val;
		binData[treeSize].cnt = 0;
		binData[treeSize].lvl = binData[up].lvl + 1;
		binData[treeSize].zero = 0;
		binData[treeSize].up = up;
		binData[treeSize].one = 0;

		if (val == 1)
			binData[up].one = treeSize;
		else
			binData[up].zero = treeSize;

		binData[treeSize].init = 0;
		treeSize++;
	}
}

unsigned int BinTree::addInSpanTree(const unsigned int &item, unsigned int pos, const unsigned int &mask)
{
	unsigned int iRet = 0;
	binData[pos].cnt++;

	if (mask == 0)
		return pos;

	if (mask & item)
	{
		if (binData[pos].one == 0)
			initElement(1, pos);

		iRet = addInSpanTree(item, binData[pos].one, mask >> 1);
	}
	else
	{
		if (binData[pos].zero == 0)
			initElement(-1, pos);

		iRet = addInSpanTree(item, binData[pos].zero, mask >> 1);
	}

	return iRet;
}

unsigned int BinTree::addItemSpanning(const unsigned int &item)
{
	unsigned int mask = 0x80000000;
	unsigned int iRet = 0;

	if (binData.size() == 0)
	{
		binData.resize(2);
		binData[0].val = -1;
		binData[0].cnt = 0;
		binData[0].lvl = 0;
		binData[0].zero = 0;
		binData[0].one = 0;

		binData[1].val = 1;
		binData[1].cnt = 0;
		binData[1].lvl = 0;
		binData[1].zero = 0;
		binData[1].one = 0;
		treeSize = 2;
	}

	if (mask & item)
		iRet = addInSpanTree(item, 1, mask >> 1);
	else
		iRet = addInSpanTree(item, 0, mask >> 1);

	return iRet;
}

unsigned int BinTree::addInTree(const unsigned int &item, const unsigned int &pos, const unsigned int &mask)
{
	unsigned int limit = 1 << (32 - depth);
	unsigned int iRet = 0;

	binData[pos].cnt++;

	if (mask < limit)
		return pos;

	//cout << ToString(mask) << endl;
	
	if (mask & item)
		iRet = addInTree(item, binData[pos].one, mask >> 1);
	else
		iRet = addInTree(item, binData[pos].zero, mask >> 1);

	return iRet;
}

unsigned int BinTree::addItem(const unsigned int &item)
{
	unsigned int mask = 0x80000000;
	unsigned int iRet = 0;
	//cout << endl << ToString(item) << endl;
	//cout << ToString(mask) << endl;

	if (mask & item)
		iRet = addInTree(item, 1, mask >> 1);
	else
		iRet = addInTree(item, 0, mask >> 1);

	return iRet;
}

string BinTree::ToString(const unsigned short &val, unsigned short mask)
{
	unsigned int i = 0, msk = 0x8000;
	string out;
	out.resize(16, ' ');

	while (msk != mask)
	{
		msk >>= 1;
		i++;
	}

	while (mask)
	{
		out[i++] = (val & mask ? '1' : '0');
		mask >>= 1;
	}

	return out; 
}

unsigned int BinTree::addInTree(const unsigned short &item, const unsigned int &pos, const unsigned short &mask)
{
	unsigned short limit = 1 << (16 - depth);
	unsigned int iRet = 0;

	binData[pos].cnt++;

	if (mask < limit)
		return pos;

	//cout << ToString(mask) << endl;
	
	if (mask & item)
		iRet = addInTree(item, binData[pos].one, mask >> 1);
	else
		iRet = addInTree(item, binData[pos].zero, mask >> 1);

	return iRet;
}

unsigned short BinTree::getMask(const unsigned int &i) const
{
	unsigned short mask = 0;
	unsigned int pos = i;
	stringstream s1, s2;

	if (binData[pos].val > 0)
		mask |= 1 << (depth - binData[pos].lvl - 1);

	while (pos > 0)
	{
		pos = binData[pos].up;

		if (binData[pos].val > 0)
			mask |= 1 << (depth - binData[pos].lvl - 1);
	}

	return mask;
}

unsigned int BinTree::addItem(const unsigned short &item)
{
	unsigned short mask = 0x8000;
	unsigned int iRet = 0;
	//cout << endl << ToString(item) << endl;
	//cout << ToString(mask) << endl;

	if (mask & item)
		iRet = addInTree(item, 1, mask >> 1);
	else
		iRet = addInTree(item, 0, mask >> 1);

	unsigned short msk = getMask(iRet);

	if (msk != item)
		int failed = 0;

	return iRet;
}