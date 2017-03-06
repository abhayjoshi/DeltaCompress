// DeltaCompress.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "Points.h"
#include "BinTree.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <list>

using namespace std;

void Test1_Int();
void Test2_Int_Span();
void Test3_Short();

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int mask = 0x80000000;
	
	float f1 = 35.34542f, f2 = 36.56342f; 
	unsigned int v1 = *(unsigned int*) ((void*) &f1);
	unsigned int v2 = *(unsigned int*) ((void*) &f2);
	//unsigned int mask = 0x80000000;
/*
	for (unsigned int i = 0; i < 32; i++)
		cout << (v1 & mask >> i ? 1 : 0) << " ";
	cout << endl;

	for (unsigned int i = 0; i < 32; i++)
		cout << (v2 & mask >> i ? 1 : 0) << " ";
	cout << endl;
	*/
	unsigned int n = sizeof(data)/sizeof(data[0]);

	BinTree b1, b1Spanning, b2;
	
	//Test1_Int();
	//Test2_Int_Span();
	Test3_Short();

	return 0;
}

void Test1_Int()
{
	unsigned int n = sizeof(data)/sizeof(data[0]);
	vector<unsigned int> indices(n, 0);
	BinTree b1;

	if (b1.initilizeBinData(32) > 0)
		return ;

	//addItem(v1);
	//addItem(v2);

	unsigned int i = 0;

	for (; i < n; i++)
	{
		indices[i] = b1.addItem(*(unsigned int*) ((void*) &(data[i]));
	}

	i = b1.binData.size() - 1;
	unsigned int cnt = 0, mask1 = 0;
	std::list<binHead32> hd1;

	do
	{
		if (b1.binData[i].cnt > 0)
		{
			cnt++;
			mask1 = 0;

			unsigned int pos = i;
			stringstream s1, s2;
			//s1 << setw(5) << b1.binData[pos].cnt;
			//s2 << setw(5) << b1.binData[pos].val;

			if (b1.binData[pos].val > 0)
				mask1 |= 1 << b1.binData[pos].lvl;

			while (pos > 0)
			{
				pos = b1.binData[pos].up;

				if (b1.binData[pos].val > 0)
					mask1 |= 1 << b1.binData[pos].lvl;

				//s1 << setw(5) << b1.binData[pos].cnt;
				//s2 << setw(5) << b1.binData[pos].val;
			}

			//cout << s1.str() << endl << s2.str() << endl;
			cout << b1.ToString(mask1) << " - " << b1.binData[i].cnt << endl;
		}
	} while (i-- > 0 && b1.binData[i].lvl > 7);
}

void Test2_Int_Span()
{
	unsigned int n = sizeof(data)/sizeof(data[0]);
	vector<unsigned int> indices(n, 0);
	BinTree b1Span;

	unsigned int i = 0;

	for (; i < n; i++)
	{
		indices[i] = b1Span.addItemSpanning(*(unsigned int*) ((void*) &(data[i]));
	}

	i = b1Span.binData.size() - 1;
	unsigned int cnt = 0, mask1 = 0;
	list<binHead32> hd1;

	ofstream f("test.txt", ofstream::out);

	while (b1Span.binData[--i].init == 1);

	do
	{
		if (b1Span.binData[i].cnt > 0)
		{
			cnt++;
			mask1 = 0;

			unsigned int pos = i;
			stringstream s1, s2;

			if (b1Span.binData[pos].val > 0)
				mask1 |= 1 << b1Span.binData[pos].lvl;

			while (pos > 0)
			{
				pos = b1Span.binData[pos].up;

				if (b1Span.binData[pos].val > 0)
					mask1 |= 1 << b1Span.binData[pos].lvl;
			}

			f << b1Span.ToString(mask1, 1 << b1Span.binData[i].lvl) << " - " << b1Span.binData[i].cnt << endl;
		}
	} while (i-- > 0);

	f.close();
}

void Test3_Short()
{
	unsigned int n = sizeof(data)/sizeof(data[0]);
	vector<unsigned int> indices(n, 0);
	BinTree b2;

	if (b2.initilizeBinData(16) > 0)
		return ;

	unsigned int i = 0;

	for (; i < n; i++)
	{
		indices[i] = b2.addItem(*(unsigned short*) ((void*) &(data[i])));
	}

	i = b2.binData.size() - 1;
	unsigned int cnt = 0;
	unsigned short mask1 = 0;
	list<binHead16> hd1;

	ofstream f("test16.txt", ofstream::out);

	do
	{
		if (b2.binData[i].cnt > 0)
		{
			cnt++;
			mask1 = 0;

			unsigned int pos = i;
			stringstream s1, s2;

			if (b2.binData[pos].val > 0)
				mask1 |= 1 << b2.binData[pos].lvl;

			while (pos > 0)
			{
				pos = b2.binData[pos].up;

				if (b2.binData[pos].val > 0)
					mask1 |= 1 << b2.binData[pos].lvl;
			}

			f << b2.ToString(mask1, 1 << b2.binData[i].lvl) << " - " << b2.binData[i].cnt << endl;
		}
	} while (i-- > 0);

	f.close();


}
