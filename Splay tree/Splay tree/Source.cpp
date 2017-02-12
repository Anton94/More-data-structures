#include <iostream>
#include "Splay tree.h"

void testInsertSearch(const unsigned SIZE)
{
	bool status = true;
	vector<int> vals(SIZE);
	SplayTree<int> st;

	printf("Test - inserting and searching...\n");

	// Insert some random elements.
	for (unsigned i = 0, k; i < SIZE; ++i)
	{
		k = rand();
		vals[i] = k;
		st.insert(k);
	}

	// Search for thouse elements.
	for (unsigned i = 0; i < SIZE; ++i)
	{
		if (!st.search(vals[i]))
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
	SplayTree<int> st;

	printf("Test - inserting and searching for invalid values...\n");

	// Insert some random POSITIVE and some negative numbers. Random - to be slightly more random situation... doesn't matter so much...
	for (unsigned i = 0, k; i < SIZE; ++i)
	{
		k = rand() % myRandMax + 1; // If I insert 0 I will search for -0 wich is 0...
		vals[i] = k;
		st.insert(k);
	}

	// Search for some invalid elements(negative numbers).
	for (unsigned i = 0; i < SIZE; ++i)
	{
		if (st.search(vals[i] + myRandMax) || st.search(-vals[i]))
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
	SplayTree<int> st;
	const unsigned REMOVE_COUNT = SIZE / 3; // Removing ~33% of the elements.
	vector<int> removed(REMOVE_COUNT);

	printf("Test - inserting and removing values...\n");

	// Insert some random UNIQUE numbers.
	for (unsigned i = 0, k; i < SIZE; ++i)
	{
		while (st.search(k = rand() * rand())) // To make the numbers in bigger range... (so it will be easyer to find new element)
			;
		vals[i] = k;
		st.insert(k);
	}

	// Remove elements.
	for (unsigned i = 0, k; i < REMOVE_COUNT; ++i)
	{

		k = rand() % vals.size();
		removed[i] = vals[k]; // Keep the removed element.

		st.remove(vals[k]);
		// not good way, but it's ok for the purposses
		vals[k] = vals.back();
		vals.pop_back();
	}

	// Search for the elements, left in the tree.
	unsigned bound = SIZE - REMOVE_COUNT;
	for (unsigned i = 0; i < bound; ++i)
	{
		if (!st.search(vals[i]))
		{
			status = false;
			break;
		}
	}

	// Search for the removed elements.
	for (unsigned i = 0; i < REMOVE_COUNT; ++i)
	{
		if (st.search(removed[i]))
		{
			status = false;
			break;
		}
	}

	if (status && st.size() == SIZE - REMOVE_COUNT)
		printf("\t...success!\n");
	else
		printf("\t...failed!\n");
}
int main()
{
	srand(NULL);
	//_CrtMemState s1, s2, s3;
	//_CrtMemCheckpoint(&s1);
	{

		testInsertSearch(1 << 16);
		testInsertSearchInvalidValues(1 << 16);
		testInsertRemoveSearch(1 << 16);

	}

	//_CrtMemCheckpoint(&s2);

	//if (_CrtMemDifference(&s3, &s1, &s2))
	//{
	//	std::cout << "Memory leak detected!" << std::endl;
	//	_CrtMemDumpStatistics(&s3);
	//}
	return 0;
}