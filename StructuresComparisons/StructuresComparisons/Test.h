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

#endif
