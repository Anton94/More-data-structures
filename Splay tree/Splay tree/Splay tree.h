#pragma once

/* 
*	Name: Anton Dudov
*	Github repository: https://github.com/Anton94/More-data-structures/tree/master/Splay%20tree
*
*	Recursive implementation of splay tree. 
*
*
*	NOTE: I added insertion with no duplication of elements just to test something...
*
*
*
*/

#include <cassert>
#include <vector>
using std::vector;

template <class T>
class SplayTree
{
	struct Node
	{
		T val;
		Node *left, *right;
		Node(const T& v)
		{
			val = v;
			left = right = NULL;
		}
	};

	Node *root;
	unsigned count;
public:
	SplayTree() { setDefaultValues(); }
	~SplayTree() { free(root); }

	// "Moves" the object to the given one, just pointer and counter moving.
	void move(SplayTree & other)
	{
		if (this == &other)
			return;

		other.root = root;
		other.count = count;

		setDefaultValues();
	}

	// Inserts new node with value @val in the tree and make rotations so this new node becomes the root.
	void insert(const T& val)
	{
		vector<Node*> path; // TODO: Initial capacity logN
		path.reserve(42);
		insert(path, root, val);
		++count;

		splay(path);
		assert(root->val == val);
		assert(checkValidTree(root));
	}

	// Inserts new node with value @val in the tree and make rotations so this new node becomes the root only if the val is not already in the tree.
	void insertNoDuplications(const T& val)
	{		
		if (searchSimple(root, val))
			return;
		insert(val);
	}

	// Removes the first node with value @val. Standart removing in BST.
	void remove(const T& val)
	{
		remove(root, val);

		assert(checkValidTree(root));
	}

	// Searches for the first node with value @val and makes rotations so this node becomes the root.
	bool search(const T& val)
	{
		vector<Node*> path; // TODO: Initial capacity logN
		path.reserve(42);
		if (search(path, root, val)) // If there is such node, make the splay routine
		{
			splay(path);
			assert(root->val == val);
			assert(checkValidTree(root));
			return true;
		}

		return false;
	}
public:
	// Returns the ~used memory for the tree. // Counting the memory for the nodes.
	unsigned getMemoryUsed() const
	{
		return count * sizeof(SplayTree::Node);
	}

	// Frees the tree.
	void free() 
	{ 
		free(root); 
		setDefaultValues(); 
	}

	bool isEmpty() const { return root; }
	unsigned size() const { return count; }
private:
	// Applies the splay routine on the given path. The last element in the path is the node I need to push to the root. @path contains at least one node!
	void splay(vector<Node*> & path)
	{
		int i = path.size() - 1; // Get the last node's index, the node I need to rotate to become a root.
		Node *n = path[i];
		
		while (i > 1)
		{ 
			// I want to see the things better for now, so I will find the real pointers and work with them for now.
			Node *& grnd = (path[i - 2] != root) ? 
				((path[i - 3]->right == path[i - 2]) ? path[i - 3]->right : path[i - 3]->left) // path[i - 3] is the grand-grand parent, to find the real pointer to the grand parent
				: root; // path[i - 2] == root (the grand parent is the root's node) so the pointer that points to the grand parent is the @root pointer.
			
			Node *& parent = (path[i - 2]->left == path[i - 1]) ?
				path[i - 2]->left // Grandparent's left child is the parent
				: path[i - 2]->right; // Grandparent's right child is the parent

			/* Four cases: (zig(left), zag(right) child)
			- @n is zig-zag
			- @n is zag-zig
			- @n is zig-zig   These two are important. They are the difference between this splay algorithm and the "naive" rotate to root method
			- @n is zag-zag   They make so that the amortized complexity to be logarithmic and not linear.
			*/
			if (grnd->left == parent) // zig
			{
				if (parent->right == n) // zig-zag
				{
					leftRotation(parent); // Apply left rotation on the parent.
					rightRotation(grnd); // Apply right rotation on the grand parent.
				}
				else // (parent->left == n) zig-zig
				{
					rightRotation(grnd); // Right rotation on the grandparent
					rightRotation(grnd); // The parent is now at the position on the grandparent and it's no more child of the grandparent, so rotate again the pointer to the grand parent(which points to the parent)
				}
			}
			else if (grnd->right == parent) // zag (symetric to the other case)
			{
				if (parent->right == n) // zag-zag
				{
					leftRotation(grnd);
					leftRotation(grnd);
				}
				else // (parent->left == n) zag-zig
				{
					rightRotation(parent);
					leftRotation(grnd);					
				}
			}

			i -= 2;
		}

		// Now there are three cases:
		if (n == root->left) //	@n is left child of the root
			rightRotation(root);
		else if (n == root->right) // @n is right child of the root
			leftRotation(root);
		// else @n is the root- OK
	}

	// Finds the place of the new element, creates it and places it. Keeps the nodes it visites in the given vector.
	void insert(vector<Node*>& path, Node *& n, const T& val)
	{
		if (!n)
		{
			n = new (std::nothrow) Node(val);
			path.push_back(n);
			return;
		}

		path.push_back(n);

		if (val < n->val)
			insert(path, n->left, val);
		else // >=
			insert(path, n->right, val);
	}

	// Returns true of the searched value is in the tree, otherwise returns false. Keeps the nodes it visites in the given vector.
	bool search(vector<Node*>& path, Node* r, const T& val) const
	{
		if (!r)
			return false;

		path.push_back(r);

		if (r->val == val)
			return true;

		if (val < r->val)
			return search(path, r->left, val);
		else
			return search(path, r->right, val);
	}

	// Returns true of the searched value is in the tree, otherwise returns false
	bool searchSimple(Node* r, const T& val) const
	{
		if (!r)
			return false;

		if (r->val == val)
			return true;

		if (val < r->val)
			return searchSimple(r->left, val);
		else // >
			return searchSimple(r->right, val);
	}

	// Removes a element @val from the tree. (if there is such element)
	void remove(Node *& r, const T& val)
	{
		if (!r)
			return; // The element is not in the tree.

		if (r->val == val) // This is the element I need to remove.
		{
			if (!r->left || !r->right) // Have one or zero childners.
			{
				Node * p = (!r->left) ? r->right : r->left;
				delete r;
				r = p;
			}
			else
			{
				// Now is the case when the element has two child subtrees.
				// Find the smaller element from the right subtree, put it in the @r->val and remove the "smaller element in the right subtree".
				r->val = removeSmallerElement(r->right);
			}
			--count;
		}
		else if (val < r->val)
			remove(r->left, val);
		else
			remove(r->right, val);
	}

	// Removes the smaller element and returns it's @val value.
	const T removeSmallerElement(Node*& r)
	{
		assert(r);

		// The smallest node(smaller value in node in the tree with root @r) for sure hase no left child.
		// So I can concatenate the @r pointer to it's right child. (it could be NULL, no problem)

		if (!r->left) // @r has the smallest value
		{
			// With share pointer maybe it will be better.
			T v = r->val;
			Node * p = r->right;
			delete r;
			r = p;

			return v;
		}

		return removeSmallerElement(r->left); // It should not call copy constructor multiple times.
	}

	// Left rotation.
	void leftRotation(Node *& p)
	{
		assert(p && p->right); // !(!p || !p->right)

		Node * a = p;
		p = p->right;
		a->right = p->left;
		p->left = a;
	}

	// Right rotation.
	void rightRotation(Node *& p)
	{
		assert(p && p->left); // !(!p || !p->left)
	
		Node * a = p;
		p = p->left;
		a->left = p->right;
		p->right = a;
	}

	// Checks if the tree is valid binary search tree.
	bool checkValidTree(Node* n) const
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
	// Sets the default values of the tree.
	void setDefaultValues()
	{
		root = NULL;
		count = 0;
	}

	// Recursive memory freeing.
	void free(Node* n)
	{
		if (!n)
			return;
		free(n->left);
		free(n->right);
		delete n;
	}
private:
	SplayTree(const SplayTree&o);
	SplayTree& operator=(const SplayTree&o);
};
