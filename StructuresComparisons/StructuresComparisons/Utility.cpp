#include "Utility.h"


// Returns some close number to log2 of the given.
unsigned closeLogTwo(unsigned x)
{
	unsigned counter = 1;
	while (x >>= 1)
		++counter;
	return counter;
}

// Writes to the @data vector increasing numbers in range[1, size]
// Returns the given vector.
vector<int>* getDataIncreasing(vector<int>* data)
{
	for (unsigned i = 0, bound = data->size(); i < bound; ++i)
		(*data)[i] = i;

	return data;
}

// Writes to the @data vector decreasing numbers in range[1, size]
// Returns the given vector.
vector<int>* getDataDecreasing(vector<int>* data)
{
	for (unsigned i = data->size(); i > 0; )
		(*data)[--i] = i;

	return data;
}

// Writes to the @data vector random ODD numbers. (odd, because if I need to search for not-existing elements, I can do it easy)
// Returns the given vector.
vector<int>* getDataRandomOdd(vector<int>* data)
{
	// Insert some random UNIQUE numbers.
	for (unsigned i = 0, bound = data->size(); i < bound; ++i)
	{
		(*data)[i] = ((rand() + 1)*((rand() + 1))) | 1;
	}

	return data;
}


// Writes to the @data vector random unique ODD numbers.
// Returns a pointer to the given vector.
vector<int>* getDataRandomOddUnique(vector<int>* data)
{
	// Insert some random UNIQUE numbers.
	for (unsigned k, i = 0, bound = data->size(); i < bound; ++i)
	{
		while (std::find(data->begin(), data->end(), k = ((rand() + 1)*((rand() + 1))) | 1) != data->end())
			;
		(*data)[i] = k;
	}

	return data;
}

// Writes to the @dataToRemove every second element to be +1(so if data has odd numbers, they will be even, so they will not be in @data vector. The others are the same.
// At the end, I will remove 50% of the elements and the others will not be found in the structure.
vector<int>* getDataToRemove(vector<int>* dataToRemove, const vector<int>* data)
{
	memcpy(&(*dataToRemove)[0], &(*data)[0], data->size() * sizeof(int)); // I can copy only the elements with even index, but it might be faster this way.

	for (unsigned i = 0, bound = dataToRemove->size(); i < bound; i += 2)
		++(*dataToRemove)[i];

	return dataToRemove;
}

// Writes to the @data vector random even numbers.
// Returns the given vector.
//vector<int>* getDataRandomEven(vector<int>* data)
//{
//	unsigned mask = (-1); // Makes it 111...1 in binary
//	mask -= 1; // makes it 111...110 in binary.
//
//			   // Insert some random UNIQUE numbers.
//	for (unsigned i = 0, bound = data->size(); i < bound; ++i)
//	{
//		(*data)[i] = ((rand() + 1)*((rand() + 1))) & mask; // makes it even
//	}
//
//	return data;
//}



// Makes matrix rows x cols and sets every cell of the matrix to zero.
void setZeroMatrix(vector<vector<unsigned>>& m, unsigned rows, unsigned cols)
{
	m.resize(rows);
	for (unsigned i = 0; i < rows; ++i)
		m[i].resize(cols, 0);
}