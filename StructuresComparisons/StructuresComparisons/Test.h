#ifndef TEST__
#define TEST__

#include <vector>
#include <string>
#include <time.h>
#include "Wrapper.h"
#include "Utility.h"
using std::vector;
using std::string;

template<class T>
class Tester
{
protected:
	vector<T> data;
public:
	// Returns the time that took to execute the test on the @tester structure and with the @data elements.
	virtual unsigned run(StructureWrapper<T> * tester) = 0;
	// Makes the data it needs for it's test from the given input data.
	virtual void setData(const vector<T>* in) = 0;
	virtual string getDescription() const = 0;
	virtual ~Tester() = 0 {} // Just in case I need something in the future.
};



template<class T>
class InsertTest : public Tester<T>
{
public:
	// Returns the time that took to execute the test on the @tester structure and with the @data elements.
	virtual unsigned run(StructureWrapper<T> * tester)
	{
		clock_t start, end;
		start = clock();
		for (unsigned i = 0, bound = data.size(); i < bound; ++i)
			tester->insert(data[i]);
		end = clock();
		return end - start;
	}

	// Makes the data it needs for it's test from the given input data.
	virtual void setData(const vector<T>* in)
	{
		// Resize the @data.
		data.resize(in->size());
		// For the insertion, I need the same data, no changes.
		memcpy(&data[0], &(*in)[0], in->size() * sizeof(T));
	}
	// Returns the description of what this test does.
	virtual string getDescription() const
	{
		return string("Insertion");
	}
	virtual ~InsertTest() {} 
};



template<class T>
class RemoveTest : public Tester<T>
{
public:
	// Returns the time that took to execute the test on the @tester structure and with the @data elements.
	virtual unsigned run(StructureWrapper<T> * tester)
	{
		clock_t start, end;
		start = clock();
		for (unsigned i = 0, bound = data.size(); i < bound; ++i)
			tester->remove(data[i]);
		end = clock();
		return end - start;
	}

	// Makes the data it needs for it's test from the given input data.
	virtual void setData(const vector<T>* in)
	{
		// Resize the @data.
		data.resize(in->size());
		// For the removing, I need the same data.
		memcpy(&data[0], &(*in)[0], in->size() * sizeof(T));

		// I will make every second element +1, so to be different from the input one and if the inputs are odd/even, it will be good, if not- it also will be good(it doesn't matter so much)
		for (unsigned i = 0, bound = data.size(); i < bound; i += 2)
			data[i]++;

		// Shuffle it.
		getDataShuffled(&data);
	}
	// Returns the description of what this test does.
	virtual string getDescription() const
	{
		return string("Removing");
	}
	virtual ~RemoveTest() {}
};


template<class T>
class SearchTest : public Tester<T>
{
public:
	// Returns the time that took to execute the test on the @tester structure and with the @data elements.
	virtual unsigned run(StructureWrapper<T> * tester)
	{
		clock_t start, end;
		start = clock();
		for (unsigned i = 0, bound = data.size(); i < bound; ++i)
			tester->search(data[i]);
		end = clock();
		return end - start;
	}

	// Makes the data it needs for it's test from the given input data.
	virtual void setData(const vector<T>* in)
	{
		// Resize the @data.
		data.resize(in->size());
		// For the removing, I need the same data.
		memcpy(&data[0], &(*in)[0], in->size() * sizeof(T));

		// I will make every second element +1, so to be different from the input one and if the inputs are odd/even, it will be good, if not- it also will be good(it doesn't matter so much)
		for (unsigned i = 0, bound = data.size(); i < bound; i += 2)
			data[i]++;

		// Shuffle it.
		getDataShuffled(&data);
	}
	// Returns the description of what this test does.
	virtual string getDescription() const
	{
		return string("Searching");
	}
	virtual ~SearchTest() {}
};

// Searches for elements in the structure, but with ~logarithmic range of elements. Searches for ~ n*log2(n) elements.
// Makes a "windows" of ~log2(n) of elements and searches random elements witheen 
template<class T>
class SearchLocalityTest : public Tester<T>
{
public:
	// Returns the time that took to execute the test on the @tester structure and with the @data elements.
	virtual unsigned run(StructureWrapper<T> * tester)
	{
		clock_t start, end;
		start = clock();
		for (unsigned i = 0, bound = data.size(); i < bound; ++i)
				tester->search(data[i]);
		end = clock();
		return end - start;
	}

	// Makes the data it needs for it's test from the given input data.
	virtual void setData(const vector<T>* in)
	{
		unsigned LOG = closeLogTwo(in->size());

		data.resize((in->size() - LOG) * LOG);

		for (unsigned j = 0, i = 0, bound = in->size() - LOG; i < bound; ++i)
			for (unsigned k = i, boundWindow = i + LOG; k < boundWindow; ++k)
				data[j++] = (*in)[i + rand() % LOG]; // Adds element with random index in range [i, i + LOG)
	}

	// Returns the description of what this test does.
	virtual string getDescription() const
	{
		unsigned LOG = closeLogTwo(data.size());
		return "Searching with locality (" + to_string(data.size()) + " searches made)";
	}
	virtual ~SearchLocalityTest() {}
};

// Searches for only ~log2(n) elements. n times
template<class T>
class SearchLocalityFixedTest : public Tester<T>
{
public:
	// Returns the time that took to execute the test on the @tester structure and with the @data elements.
	virtual unsigned run(StructureWrapper<T> * tester)
	{
		clock_t start, end;
		start = clock();
		for (unsigned i = 0, bound = data.size(); i < bound; ++i)
				tester->search(data[i]);
		end = clock();
		return end - start;
	}

	// Makes the data it needs for it's test from the given input data.
	virtual void setData(const vector<T>* in)
	{
		unsigned j = 0, LOG = closeLogTwo(in->size()),
			size = in->size();
		data.resize(size);

		for (unsigned bound = LOG; j < bound; ++j)
			data[j] = (*in)[rand() % size]; // Fixed random elements, ~log2 of them

		while (j < size)
			data[j++] = data[rand() % LOG]; // Copy element with random index in range [0, LOG)
	}

	// Returns the description of what this test does.
	virtual string getDescription() const
	{
		unsigned LOG = closeLogTwo(data.size());
		return "Searching with fixed locality (" + to_string(data.size()) + " searches made)";
	}
	virtual ~SearchLocalityFixedTest() {}
};

#endif
