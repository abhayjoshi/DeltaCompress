/**
* \file BitSet.h
* \brief CBitSet class declaration.
*
* rights
*/

#pragma once

# include <vector>

using namespace std;

//! Class CBitSet is data structure for area.
/*! Contains array of bits for area representation.
*/
class CBitSet
{
public:
	//! \fn CBitSet constructor.
	/*! default constructor
	*/
	CBitSet(void);
	~CBitSet(void);

	//! \fn setSize sets array.
	/*! sets area dimensions.
		\param width lenght of array.
	*/
	void setSize(size_t length);

	//! \fn getBit gets bit of array.
	/*! returns bit value of array.
		\param index in array.
		\param defVal default return value if bit not present
	*/
	unsigned char getBit(size_t index, unsigned char defVal = 0) const;

	//! \fn getBits gets bits of array.
	/*! returns bit value of array.
		\param index in array.
		\param sz size.
		\param defVal default return value if bit not present
	*/
	size_t CBitSet::getBits(size_t index, size_t sz, size_t defVal) const;

	//! \fn setBit sets bit of array.
	/*! sets bit of array.
		\param index in array.
		\param val value to set.
	*/
	void setBit(size_t index, unsigned char val = 1);

	//! \fn setBit sets bit of array.
	/*! sets bit of array.
		\param index in array.
		\param val value to set.
	*/
	void setBits(size_t index, size_t val = 1, size_t sz = 1);

	//! \fn setBit sets bits of array.
	/*! sets bits of array.
		\param stIndex starting index in array.
		\param edIndex ending index in array.
		\param val value to set.
	*/
	void setBit(size_t stIndex, size_t edIndex, unsigned char val = 1);

	//! \fn getLength returns array size.
	/*! returns array size.
	*/
	inline size_t getSize() const { return m_uiLength;}

private:

	vector<size_t> m_lstArea;
	size_t m_uiLength;

	static const unsigned char m_szElement;
	static const size_t m_one;
	static const size_t m_mask;
};

