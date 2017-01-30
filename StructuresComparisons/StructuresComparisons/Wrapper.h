#ifndef WRAPPER__
#define WRAPPER__

#include <string>
using std::string;

template<class T>
class StructureWrapper
{
protected:
	string description;
public:
	virtual void insert(const T& val) = 0;
	virtual bool search(const T& val) const = 0;
	virtual void remove(const T& val) = 0;
	virtual void free() = 0;
	virtual	unsigned getMemoryUsed() const = 0;
	virtual ~StructureWrapper() = 0 {};

	string getDescription() const { return description; }
	virtual void makeDescription() = 0;
};


#endif