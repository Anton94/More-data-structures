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

// Writes to the @data vector (which hase even number of elements!) on the even places some random value in [0, 100] and
// on the odd places some value outside of range with @probabilityOutsideRange probability (and with 50% probability outside "which side" it will be..)
// otherwise writes some recent value(random value of the previous @previousCount even elements)
// otherwise writes some random element in the range.
// NOTE: first applies the probability to be outside, after that to be like previous and if both fails- some random....
vector<int>* getDataRandomWithProbabilities(vector<int>* data, int a, int b, unsigned probabilityOutsideRange, unsigned probabilityRecentElements, unsigned previousCount)
{
	if (data->size() % 2 == 1) // Should not be, but in any case...
	{
		data->resize(0);
		return data;
	}

	b += 1; // So I could get random elements @b
	int previous;
	for (unsigned i = 0, bound = data->size(); i < bound; i += 2)
	{
		(*data)[i] = a + rand() % b; // set the even element.
		
		if (rand() % 100 < probabilityOutsideRange) // set the odd element to be outside of the range.
		{
			if (rand() % 100 < 50) // 50% chance to be outside the left side, and 50% chance to be outside the right side of the range
				(*data)[i + 1] = a - 1 - rand();
			else
				(*data)[i + 1] = b + 1 + rand();
		}
		else if (rand() % 100 < probabilityRecentElements) // set the odd element to be like some random element from the previous @previousCount elements.
		{
			previous = i - (rand() % previousCount);
			if (previous < 0) // Outside of the vector range, make it the first element, doesn't metter so much...
				previous = 0;
			(*data)[i + 1] = (*data)[previous];
		}
		else // It will be some random element from the range
		{
			(*data)[i + 1] = a + rand() % b;
		}
	}

	return data;
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