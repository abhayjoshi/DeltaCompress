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
#include "BitSet.h"

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
		indices[i] = b1.addItem(*(unsigned int*) ((void*) &(data[i])));
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
		indices[i] = b1Span.addItemSpanning(*(unsigned int*) ((void*) &(data[i])));
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
	unsigned int n = sizeof(data)/sizeof(unsigned short);
	vector<unsigned int> indices(n, 0);
	BinTree b2;

	if (b2.initilizeBinData(16) > 0)
		return ;

	unsigned int i = 0;
	unsigned short *in_stream = (unsigned short*) ((void *) data);

	for (; i < n; i++)
	{
		indices[i] = b2.addItem(in_stream[i]);
	}

	i = b2.binData.size() - 1;
	unsigned short mask1 = 0;
	list<binHead16> hd1;
	/*
	ofstream f("test16.txt", ofstream::out);

	do
	{
		if (b2.binData[i].cnt > 0)
		{
			mask1 = b2.getMask(i);
			f << b2.ToString(mask1, 1 << b2.binData[i].lvl) << " - " << b2.binData[i].cnt << endl;
		}
	} while (i-- > 0);

	f.close();
	*/
	CBitSet stream1, stream2;
	size_t sz = n * 32;
	const size_t intsz = sizeof(n) * 8;
	const size_t binsz = sizeof(short) * 8;
	stream1.setSize(sz);
	stream2.setSize(sz);
	stream1.setBits(stream1.getLast(), n, intsz);
	stream2.setBits(stream2.getLast(), n, intsz);

	stream1.setBits(stream1.getLast(), 0, intsz); // no of headers, init 0
	stream1.setBits(stream1.getLast(), 0, 1); // header method 0, Used headers || 1  Binary headers
	stream1.setBits(stream1.getLast(), 0, 1); // header bits method 0, same bits, don't read bits each header, next field for  header size
																 // 1, read each header bits next field for no of header size bits
	stream1.setBits(stream1.getLast(), 15, 6); // header bits
	
	// Compute used headers
	size_t nHeaders = 0;
	vector<unsigned int> found(n, 0);
	
	CBitSet tmp;
	tmp.setSize(b2.binData.size());
	vector<unsigned int> indicesNew(b2.binData.size(), 0);
	vector<unsigned short> aHeaderst(5000,0);
	size_t t = 0;

	for (i = 0; i < n; ++i)
	{
		if (tmp.getBit(indices[i]))
		{
			indices[i] = indicesNew[indices[i]];
			continue;
		}
		/*
		if (found[i] == 1)
			continue;

		for (size_t j = i + 1; j < n; ++j)
		{
			if (indices[i] == indices[j])
			{
				indices[j] = nHeaders;
				found[j] = 1;
			}
		}
		*/
		mask1 = b2.getMask(indices[i]);

		//unsigned int pos = indices[i];
		//stringstream s1, s2;

		//if (b2.binData[pos].val > 0)
		//	mask1 |= 1 << b2.binData[pos].lvl;

		//while (pos > 0)
		//{
		//	pos = b2.binData[pos].up;

		//	if (b2.binData[pos].val > 0)
		//		mask1 |= 1 << b2.binData[pos].lvl;
		//}

		tmp.setBit(indices[i]);
		indicesNew[indices[i]] = nHeaders;
		indices[i] = nHeaders++;
		//stream1.setBits(stream1.getLast(), 15, 5);
		stream1.setBits(stream1.getLast(), mask1, binsz);
		aHeaderst[t++] = mask1;
	}

	stream1.setBits(32, nHeaders, intsz); // no of headers
	
	size_t szHeaders = 0;

	while (nHeaders)
	{
		szHeaders++;
		nHeaders >>= 1;
	}

	for (i = 0; i < n; ++i)
	{
		stream1.setBits(stream1.getLast(), indices[i], szHeaders);
	}

	float *op1 = 0;
	size_t iRead = 0;
	size_t iVal = 0;
	
#define READ(s, b) s.getBits(iRead, b); iRead += b

	iVal = READ(stream1, 32);
	size_t numItems = iVal;
	iVal = READ(stream1, 32);
	size_t numHeaders = iVal;

	op1 = new float[numItems];

	// header method 0
	iVal = READ(stream1, 1);

	if (iVal != 0)
		return;
	
	// header bits method
	iVal = READ(stream1, 1);

	if (iVal != 0)
		return;

	unsigned char hSize = READ(stream1, 6);
	hSize++;
	vector<unsigned short> aHeaders(numHeaders,0);

	for (i = 0; i < numHeaders; ++i)
	{
		aHeaders[i] = READ(stream1, hSize);
	}

	szHeaders = 0;

	while (numHeaders)
	{
		szHeaders++;
		numHeaders >>= 1;
	}
	//return;
	unsigned short *op1stream = (unsigned short*) ((void *) op1);
	for (i = 0; i < numItems; ++i)
	{
		iVal = READ(stream1, szHeaders);
		op1stream[i] = aHeaders[iVal];
	}

	// Testing

	for (i = 0; i < numItems / 2; ++i)
	{
		if (op1[i] != data[i])
			int failed = 1;
	}
}
