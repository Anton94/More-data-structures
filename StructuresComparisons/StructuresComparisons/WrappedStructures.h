#ifndef WRAPPED_STRUCTURES__
#define WRAPPED_STRUCTURES__


#include "Wrapper.h"
#include "../../Binary search tree/Binary search tree/BinarySearchTree.h" // Another prject..
#include "../../Skip list/Skip list/SkipList.h"
#include "../../Splay tree/Splay tree/Splay tree.h"
#include "../../AVL tree/AVL tree/AVL.h"

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
		description = "Normal inary search tree";
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
		description = "Splay tree";
	}
};

template<class T>
class WrappedAVLTree : public StructureWrapper<T>, public AVL<T>
{
public:
	WrappedAVLTree(AVL<T> & other) { other.move(*this); makeDescription(); }
	virtual void insert(const T& val) { AVL<T>::insert(val); }
	virtual bool search(const T& val) { return AVL<T>::search(val); }
	virtual void remove(const T& val) { AVL<T>::remove(val); }
	virtual void free() { AVL<T>::free(); }
	virtual	unsigned getMemoryUsed() const { return AVL<T>::getMemoryUsed(); }
	virtual ~WrappedAVLTree() {}
private:
	virtual void makeDescription()
	{
		description = "AVL tree";
	}
};

template<class T>
class WrappedSplayTreeNoDuplications : public StructureWrapper<T>, public SplayTree<T>
{
public:
	WrappedSplayTreeNoDuplications(SplayTree<T> & other) { other.move(*this); makeDescription(); }
	virtual void insert(const T& val) { SplayTree<T>::insertNoDuplications(val); }
	virtual bool search(const T& val) { return SplayTree<T>::search(val); }
	virtual void remove(const T& val) { SplayTree<T>::remove(val); }
	virtual void free() { SplayTree<T>::free(); }
	virtual	unsigned getMemoryUsed() const { return SplayTree<T>::getMemoryUsed(); }
	virtual ~WrappedSplayTreeNoDuplications() {}
private:
	virtual void makeDescription()
	{
		description = "Splay tree with no duplications of values";
	}
};

template<class T>
class WrappedAVLTreeNoDuplications : public StructureWrapper<T>, public AVL<T>
{
public:
	WrappedAVLTreeNoDuplications(AVL<T> & other) { other.move(*this); makeDescription(); }
	virtual void insert(const T& val) { AVL<T>::insertNoDuplications(val); }
	virtual bool search(const T& val) { return AVL<T>::search(val); }
	virtual void remove(const T& val) { AVL<T>::remove(val); }
	virtual void free() { AVL<T>::free(); }
	virtual	unsigned getMemoryUsed() const { return AVL<T>::getMemoryUsed(); }
	virtual ~WrappedAVLTreeNoDuplications() {}
private:
	virtual void makeDescription()
	{
		description = "AVL tree with no duplications of values";
	}
};



#endif
