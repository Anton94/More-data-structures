#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <stack>
#include <cassert>
using std::stack;

template<class T>
struct Node
{
	Node *left, *right;
	T val;
	Node(const T& v, Node*l = NULL, Node*r = NULL) : val(v), left(l), right(r) {}
};


/*
*
*	Iterative version.
*	
*/

template<class T>
class BinarySearchTreeIterative
{
private:
	Node<T> * root;
	unsigned count;
public:
	// Creates empty tree.
	BinarySearchTreeIterative()
	{
		root = NULL;
		count = 0;
	}

	// "Moves" the object to the given one, just pointer moving.
	void move(BinarySearchTreeIterative & other)
	{
		if (this == &other)
			return;

		other.root = root;
		other.count = count;
		
		root = NULL;
		count = 0;
	}

	// Inserts element @val in the tree.
	void insert(const T& val)
	{
		insertIterative(val);
		assert(checkValidTree(root));
	}

	// Returns true of the searched value is in the tree, otherwise returns false.
	bool search(const T& val) const
	{
		return searchIterative(val);
	}

	// Removes a element @val from the tree. (if there is such element)
	void remove(const T& val)
	{
		removeIterative(val);
		assert(checkValidTree(root));
	}

	// Returns the ~memory used.
	unsigned getMemoryUsed() const
	{
		return count * sizeof(Node<T>);
	}

	// Returns true if the tree is empty, otherwise returns false. (not needed, but simple functionality)
	bool isEmpty() const
	{
		return !root; // root == NULL
	}

	unsigned size() const
	{
		return count;
	}

	// Frees the tree.
	void free()
	{
		freeIterative();
		root = NULL;
		count = 0;
	}

	// Free the memory.
	~BinarySearchTreeIterative()
	{
		freeIterative();
	}

private:
	// Finds the place of the new element, creates it and places it. Iterative
	void insertIterative(const T& val)
	{
		Node<T> ** n = &root;

		while (*n)
		{
			if (val < (*n)->val)
				n = &(*n)->left;
			else // >=
				n = &(*n)->right;
		}
		*n = new (std::nothrow) Node<T>(val);
		++count;
	}

	// Returns true of the searched value is in the tree, otherwise returns false. Iterative.
	bool searchIterative(const T& val) const
	{
		const Node<T> * const* n = &root;

		while (*n)
		{
			if (val == (*n)->val)
				return true;
			else if (val < (*n)->val)
				n = &(*n)->left;
			else // >
				n = &(*n)->right;
		}

		return false;
	}

	// Removes a element @val from the tree. (if there is such element). Iterative.
	void removeIterative(const T& val)
	{
		Node<T> **n = &root;

		while (*n)
		{
			if (val == (*n)->val)
			{
				if (!(*n)->left || !(*n)->right) // Have one or zero childners.
				{
					Node<T> * p = (!(*n)->left) ? (*n)->right : (*n)->left;
					delete *n;
					*n = p;
				}
				else // The element has two child subtrees.
				{
					// Find the smaller value from the right subtree, put it in the @*n's node and remove the "smaller element in the right subtree".
					(*n)->val = removeSmallerElementIterative(&(*n)->right);
				}
				--count;
				break;
			}
			else if (val < (*n)->val)
				n = &(*n)->left;
			else
				n = &(*n)->right;
		}
	}

	// Removes the smaller element and returns it's @val value. Iterative
	const T removeSmallerElementIterative(Node<T>** n)
	{
		// The smallest node(smaller value in node in the tree with root @r) for sure hase no left child.
		// So I can concatenate the @r pointer to it's right child. (it could be NULL, no problem)

		while (*n)
		{
			if (!(*n)->left) // Found the smallest value's node.
			{
				const T res = (*n)->val;
				Node<T> *p = (*n)->right;
				delete *n;
				*n = p;
				return res;
			}
			else
			{
				n = &(*n)->left;
			}
		}

		return T(); // Should never happened, but if so, returns some default value.
	}

	// Iterative free of the memory.
	// A little dummy way.
	void freeIterative()
	{
		if (!root)
			return;

		stack<Node<T>*> path; // Always only not NULL nodes on the path.
		path.push(root);

		Node<T> * curr;

		while (!path.empty())
		{
			curr = path.top();
			path.pop();

			if (curr->left)
				path.push(curr->left);
			if (curr->right)
				path.push(curr->right);
			delete curr;
		}
	}

	// Checks if the tree is valid binary search tree.
	bool checkValidTree(Node<T>* n) const
	{
		if (!n || (!n->left && !n->right)) // (!n->left && !n->right) one recursion level less for the leaves.
			return true;

		// Don't check for == because the rotation can mess the equal vals, the nodes can be on both sides.
		if (n->right && n->right->val < n->val)
			return false;
		if (n->left && n->left->val > n->val)
			return false;

		return checkValidTree(n->left) && checkValidTree(n->right);
	}
private:
	// Ban copy constructor and operator=
	BinarySearchTreeIterative(const BinarySearchTreeIterative& other);
	BinarySearchTreeIterative& operator=(const BinarySearchTreeIterative& other);
};


















































/*
*
*	Recursive version. it can stay here...
*
*/

template<class T>
class BinarySearchTreeRecursive
{
private:
	Node<T> * root;
	unsigned count;
public:
	// Creates empty tree.
	BinarySearchTreeRecursive()
	{
		root = NULL;
		count = 0;
	}

	// Inserts element @val in the tree.
	void insert(const T& val)
	{
		insertRecursive(root, val);
		++count;
	}

	// Returns true of the searched value is in the tree, otherwise returns false.
	bool search(const T& val) const
	{
		return searchRecursive(root, val);
	}

	// Removes a element @val from the tree. (if there is such element)
	void remove(const T& val)
	{
		removeRecursive(root, val);
	}

	// Returns true if the tree is empty, otherwise returns false. (not needed, but simple functionality)
	bool isEmpty() const
	{
		return !root; // root == NULL
	}

	unsigned size() const
	{
		return count;
	}

	// Frees the tree.
	void free()
	{
		freeRecursive(root);
		root = NULL;
	}

	// Free the memory.
	~BinarySearchTreeRecursive()
	{
		freeRecursive(root);
	}

private:
	// Finds the place of the new element, creates it and places it.
	void insertRecursive(Node<T> *& n, const T& val)
	{
		if (!n)
			n = new (std::nothrow) Node<T>(val);
		else if (val < n->val)
			insertRecursive(n->left, val);
		else // >=
			insertRecursive(n->right, val);
	}

	// Returns true of the searched value is in the tree, otherwise returns false.
	bool searchRecursive(const Node<T>* r, const T& val) const
	{
		if (!r)
			return false;

		if (r->val == val)
			return true;

		if (val < r->val)
			return searchRecursive(r->left, val);
		else
			return searchRecursive(r->right, val);
	}

	// Removes a element @val from the tree. (if there is such element)
	void removeRecursive(Node<T> *& r, const T& val)
	{
		if (!r)
			return; // The element is not in the tree.

		if (r->val == val) // This is the element I need to remove.
		{
			if (!r->left || !r->right) // Have one or zero childners.
			{
				Node<T> * p = (!r->left) ? r->right : r->left;
				delete r;
				r = p;
			}
			else
			{
				// Now is the case when the element has two child subtrees.
				// Find the smaller element from the right subtree, put it in the @r->val and remove the "smaller element in the right subtree".
				r->val = removeSmallerElementRecursive(r->right);
			}
			--count;
		}
		else if (val < r->val)
			removeRecursive(r->left, val);
		else
			removeRecursive(r->right, val);
	}

	// Removes the smaller element and returns it's @val value.
	const T removeSmallerElementRecursive(Node<T>*& r)
	{
		//if (!r)
		//	return ; // Undefined... must not happened, if so - crashes.. 

		// The smallest node(smaller value in node in the tree with root @r) for sure hase no left child.
		// So I can concatenate the @r pointer to it's right child. (it could be NULL, no problem)

		if (!r->left) // @r has the smallest value
		{
			// With share pointer maybe it will be better.
			T v = r->val;
			Node<T> * p = r->right;
			delete r;
			r = p;

			return v;
		}

		return removeSmallerElementRecursive(r->left); // It should not call copy constructor multiple times.
	}

	// Recursive memory freeing.
	void freeRecursive(Node<T> * r)
	{
		if (!r)
			return;

		freeRecursive(r->left);
		freeRecursive(r->right);
		delete r;
	}
private:
	// Ban copy constructor and operator=
	BinarySearchTreeRecursive(const BinarySearchTreeRecursive& other);
	BinarySearchTreeRecursive& operator=(const BinarySearchTreeRecursive& other);
};
#endif
