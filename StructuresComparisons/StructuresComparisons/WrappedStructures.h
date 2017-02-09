#ifndef WRAPPED_STRUCTURES__
#define WRAPPED_STRUCTURES__


#include "Wrapper.h"
#include "../../Binary search tree/Binary search tree/BinarySearchTree.h" // Another prject..
#include "../../Skip list/Skip list/SkipList.h"
#include "../../Splay tree/Splay tree/Splay tree.h"

template<class T>
class WrappedBST : public StructureWrapper<T>, public BinarySearchTreeIterative<T>
{
public:
	WrappedBST(BinarySearchTreeIterative<T> & other) { other.move(*this); makeDescription(); }
	virtual void insert(const T& val){	BinarySearchTreeIterative<T>::insert(val);}
	virtual bool search(const T& val){	return BinarySearchTreeIterative<T>::search(val);}
	virtual void remove(const T& val){	BinarySearchTreeIterative<T>::remove(val);}
	virtual void free() { BinarySearchTreeIterative<T>::free(); }
	virtual	unsigned getMemoryUsed() const{	return BinarySearchTreeIterative<T>::getMemoryUsed();}
	virtual ~WrappedBST() {}
private:
	virtual void makeDescription()
	{
		description = "Normal binary search tree";
	}
};

template<class T>
class WrappedSkipList: public StructureWrapper<T>, public SkipList<T>
{
public:
	WrappedSkipList(SkipList<T> & other) { other.move(*this); makeDescription(); }
	virtual void insert(const T& val) { SkipList<T>::insert(val); }
	virtual bool search(const T& val) { return SkipList<T>::search(val); }
	virtual void remove(const T& val) { SkipList<T>::remove(val); }
	virtual void free() { SkipList<T>::free(); }
	virtual	unsigned getMemoryUsed() const { return SkipList<T>::getMemoryUsed(); }
	virtual ~WrappedSkipList() {}
private:
	virtual void makeDescription()
	{
		description = "Skip list with " +
			to_string(getCoinProbability()) + "% coin probability and " +
			to_string(getMaxTowerHeight()) + " maximal height";
	}
};

template<class T>
class WrappedSplayTree : public StructureWrapper<T>, public SplayTree<T>
{
public:
	WrappedSplayTree(SplayTree<T> & other) { other.move(*this); makeDescription(); }
	virtual void insert(const T& val) { SplayTree<T>::insert(val); }
	virtual bool search(const T& val) { return SplayTree<T>::search(val); }
	virtual void remove(const T& val) { SplayTree<T>::remove(val); }
	virtual void free() { SplayTree<T>::free(); }
	virtual	unsigned getMemoryUsed() const { return SplayTree<T>::getMemoryUsed(); }
	virtual ~WrappedSplayTree() {}
private:
	virtual void makeDescription()
	{
		description = "Normal splay tree";
	}
};


#endif
