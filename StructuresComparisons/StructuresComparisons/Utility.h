#pragma once

#include <vector>
using std::vector;

unsigned closeLogTwo(unsigned x); // Returns some close number to log2 of the given.
vector<int>* getDataIncreasing(vector<int>* data);
vector<int>* getDataDecreasing(vector<int>* data);
vector<int>* getDataRandomOdd(vector<int>* data);
vector<int>* getDataRandomOddUnique(vector<int>* data);
//vector<int>* getDataRandomEven(vector<int>* data);


//Shuffles the data.
template<class T>
vector<T>* getDataShuffled(vector<T>* data)
{
	for (unsigned j = data->size() - 1; j > 0; --j)
		std::swap((*data)[j], (*data)[rand() % (j + 1)]);

	return data;
}

// Makes matrix rows x cols and sets every cell of the matrix to zero.
void setZeroMatrix(vector<vector<unsigned>>& m, unsigned rows, unsigned cols);