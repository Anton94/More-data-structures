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
	//runTests(testers, getDataIncreasing(&data));
	//cout << "\nInput data is SORTED in DECREASING order!\n";
	//runTests(testers, getDataDecreasing(&data));
	cout << "\nInput data is RANDOM!\n";
	runTests(testerObjects, testerFunctions, getDataRandomOdd(&data));

	// Free the memory.
	freeTesterObjects(testerObjects);
	freeTesterFunctions(testerFunctions);
}


int main()
{
	// Memory leaks cheching...
	//_CrtMemState s1, s2, s3;
	//_CrtMemCheckpoint(&s1);




	{


		testing(1 << 8);


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
	unsigned LOG_SIZE = log2(SIZE);
	unsigned unique = time(NULL); // Not so unique..
	BinarySearchTreeIterative<int> bt;
	testers.push_back(new (std::nothrow) WrappedBST<int>(bt));

	int offsetHeight = 6;

	// Different maximal tower height.
	for (int height = std::max(1, (int)LOG_SIZE - offsetHeight), bound = LOG_SIZE + offsetHeight; height <= bound; ++height)
	{
		// Different coin probability.
		for (int p = 20; p <= 70; p += 5)
		{

			SkipList<int> skipListCoinProbability(height, p, unique);
			testers.push_back(new (std::nothrow) WrappedSkipList<int>(skipListCoinProbability));
		}
	}

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
	testerFunctions.push_back(new (std::nothrow) InsertTest<int>());
	testerFunctions.push_back(new (std::nothrow) SearchTest<int>());
	testerFunctions.push_back(new (std::nothrow) RemoveTest<int>());
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

	// Print the formated data for each tested object total average time and memory.
	cout << "\n\nTotal average statistics for all tests of " << data->size() << " elements:\n\n";
	for (unsigned i = 0, bound = testerObjects.size(); i < bound; ++i)
	{
		for (unsigned j = 1, bound = testerFunctions.size(); j < bound; ++j)
		{
			times[i][0] += times[i][j] / NUMBER_OF_TESTS;
			memoryUsed[i][0] += memoryUsed[i][j] / NUMBER_OF_TESTS;
		}
		cout << "\t" << testerObjects[i]->getDescription() << ":\n";
		cout << "\t...took " << times[i][0] << " miliseconds.\n";
		cout << "\t...and memory " << memoryUsed[i][0] << " bytes.\n";
	}
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