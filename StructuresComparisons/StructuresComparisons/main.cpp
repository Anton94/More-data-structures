/*
*
*	I implemented it this way, because I want to be able with as little efford as possible to make another test for another structure(or different structure properties, like coin flip % and so)
*
*	TODO & NOTES:
*		- Memory checking for dyn allocations.
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>
#include "WrappedStructures.h"
#include "Test.h"
#include "Utility.h"
#include "../../Binary search tree/Binary search tree/BinarySearchTree.h" // Another prject..
#include "../../Skip list/Skip list/SkipList.h"
#include "../../Splay tree/Splay tree/Splay tree.h"
#include "../../AVL tree/AVL tree/AVL.h"
using std::cout;
using std::to_string;
using std::vector;

const unsigned NUMBER_OF_TESTS = 3; // How many tests to make and take their avarage

void createTesterObjects(vector<StructureWrapper<int>*> & testerObjects, unsigned SIZE);
void createTesterFunctions(vector<Tester<int>*> & testerFunctions);
void freeTesterObjects(vector<StructureWrapper<int>*> & t);
void freeTesterFunctions(vector<Tester<int>*> & t);

void testing(unsigned SIZE);
void runTests(vector<StructureWrapper<int>*> & testerObjects, vector<Tester<int>*> &testerFunctions, const vector<int>* data);
void runTestsHelperForEvenRandInRangeOddWithProbability(vector<int> &data, vector<StructureWrapper<int>*> &testerObjects, vector<Tester<int>*> &testerFunctions, int a, int b, unsigned probabilityOutsideRange, unsigned probabilityRecentElements, unsigned previousCount = 1);


// Besicaly, here I put the data sets I want to test with.
void testing(unsigned SIZE)
{
	// Here I will keep all test objects.
	vector<StructureWrapper<int>*> testerObjects;
	// Here I will keep all test functions I will use.
	vector<Tester<int>*> testerFunctions;
	// Here I will keep the input data.
	vector<int> data(SIZE);

	// Create the tester objects.
	createTesterObjects(testerObjects, SIZE);
	// Create the tester functions.
	createTesterFunctions(testerFunctions);

	// Run the tests.
	//cout << "\nInput data is SORTED in INCREASING order!\n";
	//runTests(testerObjects, testerFunctions, getDataIncreasing(&data));
	//cout << "\nInput data is SORTED in DECREASING order!\n";
	//runTests(testerObjects, testerFunctions, getDataDecreasing(&data));
	//cout << "\nInput data is RANDOM!\n";
	//runTests(testerObjects, testerFunctions, getDataRandomOdd(&data));
	
	runTestsHelperForEvenRandInRangeOddWithProbability(data, testerObjects, testerFunctions, 0, 100, 0, 100, 1);  // range [0, 100] - the odd once are like their previous elements.
	runTestsHelperForEvenRandInRangeOddWithProbability(data, testerObjects, testerFunctions, 0, 100, 0, 80, 8); // range [0, 100] - the odd once are with prob. 75% to be like some of the previos 6 elements, 25% to be some random number in the range.
	runTestsHelperForEvenRandInRangeOddWithProbability(data, testerObjects, testerFunctions, 0, 100, 50, 100, 1); // range [0, 100] - the odd once are with probability 50% to be outside of the range, 50% to be like previous once.

	// Free the memory.
	freeTesterObjects(testerObjects);
	freeTesterFunctions(testerFunctions);
}

// Runs the tests with some input vector(entries at even indexes are random in the range [a, b] and entries on odd positions depends on the probabilities to be outside of the range or like previous).
void runTestsHelperForEvenRandInRangeOddWithProbability(vector<int> &data, vector<StructureWrapper<int>*> &testerObjects, vector<Tester<int>*> &testerFunctions, int a, int b, unsigned probabilityOutsideRange, unsigned probabilityRecentElements, unsigned previousCount)
{
	cout << "\nInput data: every even element is in the range[" << a << ", " << b << "] and every odd element has\n\t"
		<< probabilityOutsideRange << "% probability to be outside of the range\n\t"
		<< probabilityRecentElements << "% probability to be like the previous " << previousCount << " element(s)!\n";
	runTests(testerObjects, testerFunctions, getDataRandomWithProbabilities(&data, a, b, probabilityOutsideRange, probabilityRecentElements, previousCount));
}
int main()
{
	//// Memory leaks cheching...
	//_CrtMemState s1, s2, s3;
	//_CrtMemCheckpoint(&s1);
	
	{
		srand(0);
		testing(1 << 23);
	}	
	
	//_CrtMemCheckpoint(&s2);

	//if (_CrtMemDifference(&s3, &s1, &s2))
	//{
	//	std::cout << "Memory leak detected!" << std::endl;
	//	_CrtMemDumpStatistics(&s3);
	//}
	return 0;
}

// Creates the tester objects. TODO - allocation check...
void createTesterObjects(vector<StructureWrapper<int>*> & testers, unsigned SIZE)
{
	unsigned LOG_SIZE = closeLogTwo(SIZE);
	unsigned unique = time(NULL); // Not so unique..

	//BinarySearchTreeIterative<int> bt;
	//testers.push_back(new (std::nothrow) WrappedBST<int>(bt));
	SplayTree<int> st;
	testers.push_back(new (std::nothrow) WrappedSplayTree<int>(st));
	AVL<int> avl;
	testers.push_back(new (std::nothrow) WrappedAVLTree<int>(avl));
	SplayTree<int> stNoDup;
	testers.push_back(new (std::nothrow) WrappedSplayTreeNoDuplications<int>(stNoDup));
	AVL<int> avlNoDup;
	testers.push_back(new (std::nothrow) WrappedAVLTreeNoDuplications<int>(avlNoDup));

	//int offsetHeight = 1;

	//// Different maximal tower height.
	//for (int height = std::max(1, (int)LOG_SIZE - offsetHeight), bound = LOG_SIZE + offsetHeight; height <= bound; ++height)
	//{
	//	// Different coin probability.
	//	for (int p = 40; p <= 60; p += 5)
	//	{

	//		SkipList<int> skipListCoinProbability(height, p, unique);
	//		testers.push_back(new (std::nothrow) WrappedSkipList<int>(skipListCoinProbability));
	//	}
	//}

	//// Fixed tower heights.
	//for (int i = 1; i <= 5; ++i)
	//{
	//	SkipList<int> skipListFixed(i, 100, unique);
	//	testers.push_back(new (std::nothrow) WrappedSkipList<int>(skipListFixed));
	//}
}

// Creates the functions I will test with.
void createTesterFunctions(vector<Tester<int>*> & testerFunctions)
{
	// NOTE: the order is important!
//	testerFunctions.push_back(new (std::nothrow) InsertTest<int>());
//	testerFunctions.push_back(new (std::nothrow) SearchTest<int>());
//	testerFunctions.push_back(new (std::nothrow) SearchLocalityTest<int>());
//	testerFunctions.push_back(new (std::nothrow) RemoveTest<int>());
	testerFunctions.push_back(new (std::nothrow) InsertEvenSearchOddTest<int>());
	//testerFunctions.push_back(new (std::nothrow) SearchLocalityFixedTest<int>(2));
	//testerFunctions.push_back(new (std::nothrow) SearchLocalityFixedTest<int>(10));
	//testerFunctions.push_back(new (std::nothrow) SearchLocalityFixedTest<int>());
}

// Runs the the given test functions with the given tester objects and the given input data.
void runTests(vector<StructureWrapper<int>*> & testerObjects, vector<Tester<int>*> & testerFunctions, const vector<int>* data)
{
	if (NUMBER_OF_TESTS <= 0 || testerObjects.size() <= 0 || testerFunctions.size() <= 0)
		return;
	vector<vector<unsigned>> times; // 2D matrix with the time took for the tests. Rows - tested object, columns - tested function.
	setZeroMatrix(times, testerObjects.size(), testerFunctions.size());
	vector<vector<unsigned>> memoryUsed; // Again, 2D matrix to keep the used memory..
	setZeroMatrix(memoryUsed, testerObjects.size(), testerFunctions.size());

	// Set data for each test function.
	for (unsigned j = 0, bound = testerFunctions.size(); j < bound; ++j)
		testerFunctions[j]->setData(data);

	// Get the time and memory needed for each test.
	for (unsigned i = 0, bound = testerObjects.size(); i < bound; ++i)
	{
		for (unsigned k = 0; k < NUMBER_OF_TESTS; ++k)
		{
			for (unsigned j = 0, bound = testerFunctions.size(); j < bound; ++j)
			{
				times[i][j] += testerFunctions[j]->run(testerObjects[i]);
				memoryUsed[i][j] += testerObjects[i]->getMemoryUsed();
			}

			testerObjects[i]->free();
		}
	}

	// Print the formated data for each test and so.
	for (unsigned j = 0, bound = testerFunctions.size(); j < bound; ++j)
	{
		cout << "\n\nRunning " << testerFunctions[j]->getDescription() << " of " << data->size() << " elements:\n\n";
		for (unsigned i = 0, bound = testerObjects.size(); i < bound; ++i)
		{
			cout << "\t" << testerObjects[i]->getDescription() << ":\n";
			cout << "\t...took " << (times[i][j] / NUMBER_OF_TESTS) << " miliseconds.\n";
			cout << "\t...and memory " << (memoryUsed[i][j] / NUMBER_OF_TESTS) << " bytes.\n";
		}
	}

	//// Print the formated data for each tested object total average time and memory.
	//cout << "\n\nTotal average statistics for all tests of " << data->size() << " elements:\n\n";
	//for (unsigned i = 0, bound = testerObjects.size(); i < bound; ++i)
	//{
	//	for (unsigned j = 1, bound = testerFunctions.size(); j < bound; ++j)
	//	{
	//		times[i][0] += times[i][j] / NUMBER_OF_TESTS;
	//		memoryUsed[i][0] += memoryUsed[i][j] / NUMBER_OF_TESTS;
	//	}
	//	cout << "\t" << testerObjects[i]->getDescription() << ":\n";
	//	cout << "\t...took " << times[i][0] << " miliseconds.\n";
	//	cout << "\t...and memory " << memoryUsed[i][0] << " bytes.\n";
	//}
}

// Deletes the allocated memory
void freeTesterObjects(vector<StructureWrapper<int>*> & t)
{
	for (unsigned i = 0, bound = t.size(); i < bound; ++i)
	{
		delete t[i];
	}
}

// Deletes the allocated memory
void freeTesterFunctions(vector<Tester<int>*> & t)
{
	for (unsigned i = 0, bound = t.size(); i < bound; ++i)
	{
		delete t[i];
	}
}