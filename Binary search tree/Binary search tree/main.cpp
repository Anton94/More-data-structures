#include <iostream>
#include <vector>
#include <random>
#include "BinarySearchTree.h"

using std::vector;

void testInsertSearch(const unsigned SIZE)
{
	bool status = true;
	vector<int> vals(SIZE);
	BinarySearchTreeIterative<int> bt;

	printf("Test - inserting and searching...\n");

	// Insert some random elements.
	for (unsigned i = 0, k; i < SIZE; ++i)
	{
		k = rand();
		vals[i] = k;
		bt.insert(k);
	}

	// Search for thouse elements.
	for (unsigned i = 0; i < SIZE; ++i)
	{
		if (!bt.search(vals[i]))
		{
			//printf("%d not found in the binary tree...\n", vals[i]);
			status = false; 
			break;
		}
	}

	if (status)
		printf("\t...success!\n");
	else
		printf("\t...failed!\n");
}

// Test search for not existing element.
void testInsertSearchInvalidValues(const unsigned SIZE)
{
	const int myRandMax = (2 << 15);
	bool status = true;
	vector<int> vals(SIZE);
	BinarySearchTreeIterative<int> bt;

	printf("Test - inserting and searching for invalid values...\n");

	// Insert some random POSITIVE and some negative numbers. Random - to be slightly more random situation... doesn't matter so much...
	for (unsigned i = 0, k; i < SIZE; ++i)
	{
		k = rand() % myRandMax + 1; // If I insert 0 I will search for -0 wich is 0...
		vals[i] = k;
		bt.insert(k);
	}

	// Search for some invalid elements(negative numbers).
	for (unsigned i = 0; i < SIZE; ++i)
	{
		if (bt.search(vals[i] + myRandMax) || bt.search(-vals[i]))
		{
			printf("%d", vals[i]);
			status = false; 
			break;
		}
	}

	if (status)
		printf("\t...success!\n");
	else
		printf("\t...failed!\n");
}


// Test for inserting, removing some of the values and searching.
void testInsertRemoveSearch(const unsigned SIZE)
{
	bool status = true;
	vector<int> vals(SIZE);
	BinarySearchTreeIterative<int> bt;
	const unsigned REMOVE_COUNT = SIZE / 3; // Removing ~33% of the elements.
	vector<int> removed(REMOVE_COUNT);

	printf("Test - inserting and searching for invalid values...\n");

	// Insert some random UNIQUE numbers.
	for (unsigned i = 0, k; i < SIZE; ++i)
	{
		k = rand(); 
		while (bt.search(k = rand() * rand())); // To make the numbers in bigger range... (so it will be easyer to find new element)
		vals[i] = k;
		bt.insert(k);
	}

	// Remove elements.
	for (unsigned i = 0, k; i < REMOVE_COUNT - 5; ++i)
	{

		k = rand() % vals.size();
		removed[i] = vals[k]; // Keep the removed element.

		bt.remove(vals[k]);
		// not good way, but it's ok for the purposses
		vals[k] = vals.back();
		vals.pop_back();
	}

	// Search for the elements, left in the tree.
	unsigned bound = SIZE - REMOVE_COUNT;
	for (unsigned i = 0; i < bound ; ++i)
	{
		if (!bt.search(vals[i]))
		{
			status = false;
			break;
		}
	}

	// Search for the removed elements.
	for (unsigned i = 0; i < REMOVE_COUNT - 5; ++i)
	{
		if (bt.search(removed[i]))
		{
			status = false;
			break;
		}
	}

	if (status)
		printf("\t...success!\n");
	else
		printf("\t...failed!\n");
}

// If it fails- it will crash... 
void testSortedInsertion(unsigned SIZE)
{
	BinarySearchTreeIterative<int> bt;

	printf("Test - inserting and searching for invalid values...\n");
	for (unsigned i = 0; i < SIZE; ++i)
		bt.insert(i);

	printf("\t...success!\n");
}

int main()
{
	srand(NULL);
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
	{

		testInsertSearch(1 << 15);
		testInsertSearchInvalidValues(1 << 15);
		testInsertRemoveSearch(1 << 14);

		testSortedInsertion(1 << 15);
	}


	///* I needed to find some bug...*/
	//BinarySearchTreeIterative<int> it;
	//BinarySearchTreeRecursive<int> r;
	//unsigned SIZE = 1 << 16;
	//vector<int> v(SIZE);
	//for (unsigned k, i = 0; i < SIZE; ++i)
	//{
	//	k = rand() % 1024;
	//	it.insert(k);
	//	r.insert(k);
	//	v[i] = k;
	//}
	//printf("%d vs %d\n", it.size(), r.size());
	//for (unsigned i = 0; i < SIZE/2; ++i)
	//{
	//	it.remove(v[i]);
	//	r.remove(v[i]);
	//}
	//printf("%d vs %d\n", it.size(), r.size());

	_CrtMemCheckpoint(&s2);

	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		std::cout << "Memory leak detected!" << std::endl;
		_CrtMemDumpStatistics(&s3);
	}

	return 0;
}