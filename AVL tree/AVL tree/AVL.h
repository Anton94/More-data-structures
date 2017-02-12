#ifndef AVL__
#define AVL__

/*
*
*	Name: Anton Dudov
*	Github repository: https://github.com/Anton94/More-data-structures/tree/master/AVL%20tree
*
*	
*	source I used to figure out the details about the structure - "Algorithms and data structures in C++ (Leendert Ammeraal)"
*	
*
*	
*	The implementation does not work with multiple same values. So there are few ways I can modify it.
*	keep more than one value in the node in linked list 
*	or keep the value and number of times it accures 
*	or search for the NODE I need to remove, give it to the remove function and removing of NODE (looking for matching the pointers of the nodes and not only the value in the node)
*	or ...
*	OR (in this case) standard removing and if the needed node to delete has two childs, swaps it with the rightmost node of it's left child tree
*	and with another routine deletes it(the node has no right child, so easy to delete) (the routine looks for NODE, not only value)
*	This way have an advantage - if someone keeps pointers to some nodes of the tree, when I remove a element, and if I swapped only the values, some other node with some other "old" value could be removed and those who keep pointers to nodes will get unexpected value change and unexpected node removed.
*	If I keep the multiple values in some other structure in the node it will be better, but I want to implement it this way, because I have not implement it before, for "exprecience" purposes...
*
*
*	I added insertion with no duplications to test something..
*/

#include <cassert>
#include <iostream>
using std::swap;

template<class T>
class AVL
{
	struct Node
	{
		T val;
		Node *left, *right;
		char height; // I need only 3 bits.. height of right - height of left. (-2 -1 0 1 2 values)
		Node(const T& v) : val(v), left(NULL), right(NULL), height(0) {}
	};

	Node *root;
	unsigned count;
public:
	AVL() { setDefaultValues(); }
	~AVL() { free(); }

	// Moves the data for the @this tree to the given one. NOTE: the given one must be empty. (does not free's the given one)
	void move(AVL& other)
	{
		if (this == &other)
			return;

		other.root = root;
		other.count = count;

		setDefaultValues();
	}

	// Returns the number of elements in the tree.
	unsigned size() const {	return count; }

	// Returns the approximate used memory.
	unsigned getMemoryUsed() const { return count * sizeof(Node); }

	// Frees the tree.
	void free()
	{
		free(root);
		setDefaultValues();
	}
public:
	// Inserts the element @val into the tree.
	void insert(const T& val)
	{
		insert(root, val);
		++count;
		assert(checkValidBSTandHeights(root));
	}

	// Inserts the element @val into the tree only if the val is not already in the tree.
	void insertNoDuplications(const T& val)
	{
		if (search(val))
			return;
		insert(val);
	}

	// Returns true if the @val is in the tree.
	bool search(const T& val)
	{
		return search(root, val);
	}

	// Removes the element @val from the tree.
	void remove(const T& val)
	{
		remove(root, val);
		assert(checkValidBSTandHeights(root));
	}
private:
	// Finds the place of the new element, creates it and places it. Keeps it balanced
	// Returned value - 0 no increasing of the height of the subtree with root @n, 1-increased height of the subtree.
	// Note to myself: When inserting, at most two rotations, 
	// because the rotation is done when the childs have different heights and inserting in the bigger one, so it will rotate it and move one element from the bigger one to the smaller one and it will have again the same real height as before inserting.
	unsigned insert(Node *& n, const T& val)
	{
		unsigned offset = 0;
		if (!n)
		{
			n = new (std::nothrow) Node(val);
			if (!n)
				return 0;
			offset = 1;
		}
		else if (val >= n->val && insert(n->right, val)) // The element was inserted in the right subtree and it's height hase been increased.
		{
			++(n->height); // +1 because the height of right subtree hase been increased (insertion returns 1). (height = height(right) - height(left)
			if (n->height == 1) // Old height has been 0, and now it's increased.
			{
				offset = 1; // This subtree is valid, but some parents might get unbalanced
			}
			else if (n->height == 2)
			{
				if (n->right->height == -1) // If the right-left subtree has bigger height than the right-right subtree
					rightRotation(n->right); // Rotates it so the right-right subtree will have bigger height than right-left subtree
				
				leftRotation(n); // Now left rotation on @n (when the inserted element is in right-right subtree) will decrease the height (and the old HEIGHT has been some X, after insertion becomes X+1 and after left rotation it will be again X, so no need for further adjustments on the back path to the root)
			}
		}
		else if (val < n->val && insert(n->left, val)) // Symetrical to the other case.
		{
			--(n->height);
			if (n->height == -1)
			{
				offset = 1;
			}
			else if (n->height == -2)
			{
				if (n->left->height == 1)
					leftRotation(n->left);

				rightRotation(n);
			}
		}

		return offset;
	}

	// Returns 1 if the heigh has been degreased, otherwise 0.
	// Note: the rotation here(after removing the element) can decrease the previous height of the subtree, so here it can affect some nodes back to the root.
	unsigned remove(Node *& n, const T& val)
	{
		if (!n)
			return 0; // Element not found.

		unsigned offset = 0;

		if (n->val == val) // Found the element
		{
			Node *p, **q;
			--count;
			if (!n->right) // No right child.
			{
				p = n;
				n = n->left;
				delete p;
				offset = 1;
			}
			else if (!n->left) // No left child.
			{
				p = n;
				n = n->right;
				delete p;
				offset = 1;
			}
			else // Has two childs.
			{
				p = n;
				q = &(n->left);
				while ((*q)->right) // Find the rightmost node of left subtree of @n
					q = &(*q)->right;

				/* In order to swap the nodes I need to swap their heights and redirect three pointers: right child, left child and pointer to the node.*/
				swap(n->height, (*q)->height); // Swap the heights of the nodes.
				
				(*q)->right = n->right; // Redirect right childs - Copies the right child of @n to the right child of @*q ((*q)->right == NULL)
				n->right = NULL;

				if (n->left != *q) 
				{
					swap(n->left, (*q)->left); // Swap their left childs.
					swap(n, *q); // Redirect the pointer to the nodes(swap them).
				}
				else // (n->left == *q) - Here simple swap of pointers can not work, because the left child of @n is also the pointer to @q.
				{
					// Redirect the left pointers.
					Node *qq = n->left;
					n->left = qq->left;
					qq->left = n;

					n = qq; // Redirect the pointer to @n (note, the pointer to *q is n->left and it's already redirected)
				}

				if (remove(n->left, p)) // Deletes the rightmost node (of the left child of @n), which is the old @n(the node I need to delete) 
					removeRoutineLeft(n, offset); // If the tree height decreased - calls left routine (I removed it from the left child of @n, so left routine)
			}
		}
		else if (val > n->val && remove(n->right, val))
			removeRoutineRight(n, offset);
		else if (val < n->val && remove(n->left, val)) // Symetric
			removeRoutineLeft(n, offset);

		return offset;
	}

	// Removing NODE of the subtree of @n. Analogous to the remove of @value, but here the toDel element has no right child!
	unsigned remove(Node *& n, const Node * toDel)
	{
		if (!n)
			return 0; // Element not found.
		
		unsigned offset = 0;
		
		if (n == toDel) // Found the Node, it has no right child, so easy to delete.
		{
			assert(n->right == NULL);
			Node *p = n;
			n = n->left;
			delete p;
			offset = 1;
		}
		else if (toDel->val > n->val && remove(n->right, toDel))
			removeRoutineRight(n, offset);
		else if (toDel->val < n->val && remove(n->left, toDel))
			removeRoutineLeft(n, offset);

		return offset;
	}

	// Helper function. Inserted in right subtree and height decreses. Changes @offset to 0 if no height decreasing, 1 if there is height decreasing
	void removeRoutineRight(Node *& n, unsigned& offset)
	{
		--(n->height); // height of the right subtree has been decreased. (remove(n->right, val) returned 1)
		if (n->height == 0)
		{
			offset = 1;  // The height of @n's subtree has been 1, now it's one level less.
		}
		else if (n->height == -2) // Left subtree has two levels more than right subtree
		{// I need longer path on left-left, so I can rotate @n(right) and move one element from left child tree to right child tree of @n
			if (n->left->height == 1) // left child has longer path on it's right child, so i need to rotate it.
				leftRotation(n->left);
			rightRotation(n);

			if (n->height == 0)
				offset = 1; // Offset was -1, after removing becomes -2 and now is 0, so the height of the subtree(of @n) has decreased.
		}
	}
	// Helper function. (Inserted in left subtree and height decreses). Changes @offset to 0 if no height decreasing, 1 if there is height decreasing
	void removeRoutineLeft(Node *& n, unsigned & offset) // Symetric to removeRoutineRight
	{
		++(n->height);
		if (n->height == 0)
		{
			offset = 1;
		}
		else if (n->height == 2)
		{
			if (n->right->height == -1)
				rightRotation(n->right);
			leftRotation(n);

			if (n->height == 0)
				offset = 1;
		}
	}

	// Returns true of the searched value is in the tree, otherwise returns false.
	bool search(Node* r, const T& val) const
	{
		if (!r)
			return false;

		if (r->val == val)
			return true;

		if (val < r->val)
			return search(r->left, val);
		else
			return search(r->right, val);
	}

	void leftRotation(Node *& p)
	{
		assert(p && p->right);

		Node * a = p;
		p = p->right;
		a->right = p->left;
		p->left = a;

		--(a->height); 
		if (p->height > 0)
			a->height -= p->height;
		--(p->height);
		if (a->height < 0)
			p->height += a->height;
	}

	void rightRotation(Node *& p)
	{
		assert(p && p->left);

		Node * a = p;
		p = p->left;
		a->left = p->right;
		p->right = a;

		++(a->height);
		if (p->height < 0)
			a->height -= p->height;
		++(p->height);
		if (a->height > 0)
			p->height += a->height;
	}
private:
	// Sets default values for the member variables...
	void setDefaultValues()
	{
		root = NULL;
		count = 0;
	}

	// Recursive freeing.
	void free(Node * n)
	{
		if (!n)
			return;

		free(n->left);
		free(n->right);
		delete n;
	}

	// Checks if the tree is valid binary search tree with valid values for the heights.
	bool checkValidBSTandHeights(Node* n) const
	{
		if (!n || (!n->left && !n->right)) // (!n->left && !n->right) one recursion level less for the leaves.
			return true;

		if (n->height < -1 || n->height > 1) // n->geight is not -1, 0 or 1
			return false;
		// Don't check for == because the rotation can mess the equal vals, the nodes can be on both sides.
		if (n->right && n->right->val < n->val)
			return false;
		if (n->left && n->left->val > n->val)
			return false;

		return checkValidBSTandHeights(n->left) && checkValidBSTandHeights(n->right);
	}
private:
	AVL(const AVL& o);
	AVL& operator=(const AVL& o);
};

#endif