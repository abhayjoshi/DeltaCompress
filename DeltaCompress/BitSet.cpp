/**
* \file BitSet.cpp
* \brief CBitSet class implementation.
*
* rights
*/

#include "StdAfx.h"
#include "BitSet.h"


const unsigned char CBitSet::m_szElement = sizeof(size_t) * 8;
const size_t CBitSet::m_one = 1;
const size_t CBitSet::m_mask = ~((size_t) 0);


CBitSet::CBitSet(void)
	:m_uiLength(0),m_uiLast(0)
{
}

CBitSet::~CBitSet(void)
{
}

void CBitSet::setSize(size_t length)
{
	m_uiLength = length;
	size_t nSize = (size_t) ceil((double) m_uiLength / m_szElement);
	m_lstArea.resize(nSize, 0);
}

unsigned char CBitSet::getBit(size_t index, unsigned char defVal) const
{
#ifdef _DEBUG
	if (index < 0 || index >= m_uiLength)
		return defVal;
#endif

	size_t iArrPos = index / m_szElement;
	unsigned char iOffset = index % m_szElement;
	unsigned char iOffsetRev = m_szElement - iOffset - 1;
	size_t one = 1;
	return ((m_lstArea[iArrPos] & (one << iOffsetRev)) > 0 ? 1 : 0);
}

size_t CBitSet::getBits(size_t index, size_t sz, size_t defVal) const
{
#ifdef _DEBUG
	if (sz < 0 ||index < 0 || index + sz > m_uiLength)
		return defVal;
#endif
	size_t ret = 0;

	//for (size_t i = 0; i < sz; i++)
	for (unsigned int i = sz; i > 0; --i)
	{
		ret = ret << 1;
		ret |= getBit(index++, defVal);
	}

	return ret;
}

void CBitSet::setBit(size_t index, unsigned char val)
{
#ifdef _DEBUG
	if (index < 0 || index >= m_uiLength)
		return;
#endif

	m_uiLast = max(m_uiLast, index + 1);
	size_t iArrPos = index / m_szElement;
	size_t iOffset = index % m_szElement;
	size_t iOffsetRev = m_szElement - iOffset - 1;

	if (val)
		m_lstArea[iArrPos] = m_lstArea[iArrPos] | (m_one << iOffsetRev);
	else
	{
		size_t offVal = m_one << iOffsetRev;
		offVal = ~offVal;
		m_lstArea[iArrPos] = m_lstArea[iArrPos] & offVal;
	}
}

void CBitSet::setBits(size_t index, size_t val, size_t sz)
{
#ifdef _DEBUG
	if (index < 0 || index >= m_uiLength)
		return;
#endif

	for (unsigned int i = sz; i > 0; --i)
	{
		setBit(index++, (unsigned char) ((val & (m_one << (i - 1))) > 0 ? 1 : 0));
	}
}

void CBitSet::setBit(size_t stIndex, size_t edIndex, unsigned char val)
{
#ifdef _DEBUG
	if (stIndex > edIndex || stIndex < 0 || edIndex >= m_uiLength)
		return;
#endif
	m_uiLast = max(m_uiLast, edIndex + 1);
	size_t idiff = edIndex - stIndex;
	size_t iArrPos = stIndex / m_szElement;
	size_t iOffset = stIndex % m_szElement;
	size_t iOffsetRev = m_szElement - iOffset;
	size_t offVal = m_one << iOffsetRev;
	offVal -= 1;
		
	do 
	{
		if (iOffsetRev < idiff)
		{
			if (val)
				m_lstArea[iArrPos] = m_lstArea[iArrPos] | offVal;
			else
			{
				offVal = ~offVal;
				m_lstArea[iArrPos] = m_lstArea[iArrPos] & offVal;
			}

			idiff -= iOffsetRev;
			iArrPos++;
			offVal = ~m_mask;
			iOffsetRev = m_szElement;
		}
		else
		{
			size_t offVal1 = m_one << (iOffsetRev - idiff - 1);
			offVal1 -= 1;
			offVal1 = ~offVal1;
			offVal &= offVal1;

			if (val)
				m_lstArea[iArrPos] = m_lstArea[iArrPos] | offVal;
			else
			{
				offVal = ~offVal;
				m_lstArea[iArrPos] = m_lstArea[iArrPos] & offVal;
			}
	
			idiff = 0;
		}
	} while (idiff > 0);
}

void CBitSet::pushBits(size_t val, size_t sz)
{
	setBits(m_uiLast, val, sz);
}
