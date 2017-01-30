#include <stdint.h>
#include <vector>
#include <random>
#include <fstream>
#include "SkipList.h"



/*
*
*	This is a "test field", not effective(most of it), but just to find out if my assumptions are correct
*	
*	and to see various statistics for the data structure and how it behaves in different probabilities.
*
*/

using std::vector;

void testInsertSearch(const unsigned SIZE)
{
	bool status = true;
	vector<int> vals(SIZE);

	printf("Test - inserting...\n");

	// Insert some random elements.
	for (unsigned i = 0, k; i < SIZE; ++i)
	{
		k = rand();
		vals[i] = k;
	}

	SkipList<int> skipList;

	for (unsigned i = 0; i < SIZE; ++i)
	{
		skipList.insert(vals[i]);
	}

	// Search for thouse elements.
	for (unsigned i = 0; i < SIZE; ++i)
	{
		if (!skipList.search(vals[i]))
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

	printf("Test - inserting and searching...\n");

	// Insert some random POSITIVE and some negative numbers. Random - to be slightly more random situation... doesn't matter so much...
	for (unsigned i = 0, k; i < SIZE; ++i)
	{
		k = rand() % myRandMax + 1; // If I insert 0 I will search for -0 wich is 0...
		vals[i] = k;
	}

	SkipList<int> skipList;

	for (unsigned i = 0; i < SIZE; ++i)
	{
		skipList.insert(vals[i]);
	}

	// Search for some invalid elements(negative numbers).
	for (unsigned i = 0; i < SIZE; ++i)
	{
		if (skipList.search(vals[i] + myRandMax) || skipList.search(-vals[i]))
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
	SkipList<int> skipList;
	const unsigned REMOVE_COUNT = SIZE / 3; // Removing ~33% of the elements.
	vector<int> removed(REMOVE_COUNT);

	printf("Test - inserting and removing...\n");

	// Insert some random UNIQUE numbers.
	for (unsigned i = 0, k; i < SIZE; ++i)
	{
		k = rand();
		while (skipList.search(k = rand() * rand())); // To make the numbers in bigger range... (so it will be easyer to find new unique element)
		vals[i] = k;
		skipList.insert(k);
	}

	// Remove elements.
	for (unsigned i = 0, k; i < REMOVE_COUNT; ++i)
	{
		k = rand() % vals.size();
		removed[i] = vals[k]; // Keep the removed element.

		skipList.remove(vals[k]);
		// not good way, but it's ok for the purposses
		vals[k] = vals.back();
		vals.pop_back();
	}

	// Search for the elements, left in the tree.
	unsigned bound = SIZE - REMOVE_COUNT;
	for (unsigned i = 0; i < bound; ++i)
	{
		if (!skipList.search(vals[i]))
		{
			status = false;
			break;
		}
	}

	// Search for the removed elements.
	for (unsigned i = 0; i < REMOVE_COUNT; ++i)
	{
		if (skipList.search(removed[i]))
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


// Simple test to print some values and tower heights.
void testWithPrintOfTheValues(unsigned SIZE)
{
	SkipList<int> skipList;

	for (unsigned i = 0; i < SIZE; ++i)
	{
		skipList.insert(i);
	}
}


// Simple test to check how much memory is used from the skip list for @SIZE elements, max heights, probability and some additional stats.
void testMemoryUsed(unsigned SIZE, int max, unsigned prob)
{
	SkipList<int> skipList(max, prob);

	struct testNode
	{
		int k;
		testNode * next;
	};
	for (unsigned i = 0; i < SIZE;)
	{
		skipList.insert(i++);
	}
	printf("Skip list with %d max tower height, %d coint probability and %d elements:\n", max, prob, SIZE);
	printf("\t... ~ %d bytes used.\n", skipList.getMemoryUsed(), SIZE);
	printf("\t... ~ %d bytes for one height towers.\n", SIZE * skipList.getMemoryForNode());
	printf("\t... ~ %d bytes for single linked list.\n\n", SIZE * sizeof(testNode));
}

// Prints the skip list's tower heights statistics for a skip list with given ammount of elements, max heights and coin probability.
void testTowerHeights(unsigned SIZE, int max, unsigned prob)
{
	SkipList<int> skipList(max, prob);
	for (unsigned i = 0; i < SIZE;)
	{
		skipList.insert(i++);
	}
	printf("Skip list with %d max tower height, %d coint probability and %d elements:\n", max, prob, SIZE);
	printf("Tower statistics:\n");
	skipList.printTowerHeightsStatistics();

}

// Makes statistic for the number of steps needed to do to finds some random element.
// Writes it to file - stats.txt (i wan't it fixed one). (No checking for file errors, writing big chuncks and such)
void testSearchingAmmountOfStepsStatistics(bool forHistogram, unsigned SIZE, int max, unsigned prob, unsigned COUNT_SEARCHES, unsigned probabilityToSearchExistingElement = 50)
{
	SkipList<int> skipList(max, prob);
	vector<int> stats(SIZE + 1); // Maximum number of steps is linear, so @SIZE.
	memset(&stats[0], 0, (SIZE + 1) * sizeof(int));
	SIZE *= 2;
	for (unsigned i = 1; i <= SIZE; i+= 2) // I will insert only odd numbers
	{
		skipList.insert(i);
	}

	// I can use hashtable, but it's for simple test, so I don't mind some extra memory.
	
	for (unsigned i = 0, k; i < COUNT_SEARCHES; ++i)
	{
		k = ((rand() + 1)*((rand() + 1))) % SIZE; // + 1 because if one of the rands returns 0, other one doesn't matter and it's not good

		if (rand() % 100 < probabilityToSearchExistingElement) // I will search some random existing item (so odd number)
			stats[skipList.searchCountJumps(k | 1)]++; 
		else
			stats[skipList.searchCountJumps(k & 1 == 0 ? k : k - 1)]++; // (even number)
	}

	SIZE /= 2;

	std::ofstream o("stats.txt", std::ofstream::out);
	if (forHistogram)
	{
		// This way I can copy them and place them into some histogram...
		for (unsigned i = 0; i <= SIZE; ++i)
			while (stats[i]-- > 0)
				o << i << "\n";
	}
	else
	{
		o << "Skip list with " << max << " max tower height, " << prob << " coins probability and " << SIZE << " elements\n";
		o << "Made " << COUNT_SEARCHES << " searches with " << probabilityToSearchExistingElement << " probability to search existing element:\n";

		// Print the stats.
		for (unsigned i = 0; i <= SIZE; ++i)
			if (stats[i] > 0)
				o << i << " steps took to find " << stats[i] << " elements\n";

		o << "\n";
	}
	o.close();
}
int main()
{
	srand(NULL);


	//testTowerHeights(1000, log2(1000000), 50);
	//testTowerHeights(1000000, log2(1000000), 50);

	//testTowerHeights(1 << 20, log2(1 << 25), 50);
	//testSearchingAmmountOfStepsStatistics(false, 1 << 15, 17, 50, 1 << 15);

	//testTowerHeights(1000, log2(1000000), 60);
	//testTowerHeights(1000000, log2(1000000), 60);

	//testWithPrintOfTheValues(150);

	for(unsigned i = 15; i <= 20; ++i)
		testMemoryUsed(1 << i, log2(1 << i), 50);

	testInsertSearch(1 << 21);
	testInsertSearchInvalidValues(1 << 22);
	testInsertRemoveSearch(1 << 20 );

	return 0;
}