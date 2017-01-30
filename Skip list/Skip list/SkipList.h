#ifndef SKIP_LIST__
#define SKIP_LIST__

#include <random>
#include <cstring>
#include <cassert>
#include <iostream>
using std::cout;



/* 
 *
 * TODO & NOTES: 
 *	- with fixed starting node(root, with some invalid value) may be better in some cases.(deleting always the smalles element, for example)
 *	- for the memory used, I don't need to call every time sizeof, but it's ok for testing purposses(curiosly).
 *	- the array of tower heights is just from curiosity.
 *	- using of another random generator!
 */

template<class T>
class SkipList
{
	struct Node
	{
		Node** tower; // My "next" pointer of the linked list is tower[0]
		unsigned count;
		T val;
		// Node* next; 
		// I can separate the "next" pointer from tower pointers, and if I have to iterate over the whole list, it might be faster, because I won't need to fetch a whole chunck of the tower array just to access one element and some usefull stuf might get rid of the caches.
		// But I won't use this iteration functionality, so I don't need it for now.
		Node(const T& v, const unsigned c)
		{
			val = v;
			count = c;
			tower = new (std::nothrow) Node*[count]; // TODO- check if NULL and do stuff...
		}
	
		// Sets all pointers in the tower to NULL.
		void setTowerToNulls()
		{
			memset(tower, 0, sizeof(Node*) * count);
		}

		~Node()
		{
			delete[] tower;
		}
	};

	unsigned maxTowerHeight;
	unsigned coinProbability; // Values in range [0, 100]
	Node* root;
	unsigned memoryTaken; // In bytes.
	unsigned * towerHeightsStatistics; // I will keep the 
public:
	SkipList(unsigned maxTowerHeight = 42, unsigned coinProbability = 50, unsigned seed = 0)
	{
		root = NULL;
		memoryTaken = 0; // I will ignore the private variables.
		setMaxTowerHeight(maxTowerHeight);
		setCoinProbability(coinProbability);
		srand(seed);

		createTowerHeightStatistics();
	}
	
	// "Moves" the object to the given one, just pointer moving and variables.
	void move(SkipList & other)
	{
		if (this == &other)
			return;

		other.free();
		delete[] other.towerHeightsStatistics;
		other.root = root;
		other.maxTowerHeight = maxTowerHeight;
		other.coinProbability = coinProbability;
		other.memoryTaken = memoryTaken;
		other.towerHeightsStatistics = towerHeightsStatistics;

		root = NULL;
		memoryTaken = 0;
		createTowerHeightStatistics();
	}

	// Deletes the allocated memory for the list and resets the varuables.
	void free()
	{
		free(root);
		root = NULL;
		memoryTaken = 0;
		memset(towerHeightsStatistics, 0, (maxTowerHeight + 1) * sizeof(unsigned));
	}

	~SkipList()
	{
		free(root);
		delete[] towerHeightsStatistics;
	}

	// Returns the coin probability.
	unsigned getCoinProbability() const
	{
		return coinProbability;
	}

	// Returns the max tower height.
	unsigned getMaxTowerHeight() const
	{
		return maxTowerHeight;
	}
private:
	// Sets the max tower height, but if there are some elements in the list, their height won't change!
	void setMaxTowerHeight(unsigned maxTowerHeight)
	{
		this->maxTowerHeight = maxTowerHeight <= 0 ? 1 : maxTowerHeight; // The towers must be at least 1 height, because of the next pointer of the list.
		 //TODO- resize the tower heights statistics.
		 //TODO- resize first node's tower
	}

	// Sets the probability of making another level of the tower. (probability of flipping a coin to match the needed side)
	// The probability shoud be between 0 and 100, if it's more than 100, I will set it to 100, it's OK for me.
	void setCoinProbability(unsigned coinProbability)
	{
		this->coinProbability = (coinProbability > 100) ? 100 : coinProbability;
	}
public: // (debugging&curiosly purposes)
	// Returns the ~ memory used. 
	unsigned getMemoryUsed() const
	{
		return memoryTaken;
	}

	// Returns the ~ memory used. (debugging&curiosly purposes)
	void printTowerHeightsStatistics() const
	{
		for (unsigned i = 1; i <= maxTowerHeight; ++i) // not possible tower with 0 levels, at least 1.
			printf("Towers with %d floors = %d\n", i, towerHeightsStatistics[i]);
	}

	// Returns the size of single node.
	unsigned getMemoryForNode() const
	{
		return sizeof(Node);
	}

	// Returns the number of "jumps" it makes till it finds (or not) the searched value.
	unsigned searchCountJumps(const T& val) const
	{
		unsigned jumps = 0;
		if (!root)
			return jumps;
		if (root->val == val)
			return jumps;

		Node *p = root;
		int level = p->count - 1;

		while (level >= 0)
		{
			++jumps;
			if (!p->tower[level] || p->tower[level]->val > val)
			{
				--level; // Search one level down in the same tower.
			}
			else if (p->tower[level]->val < val)
			{
				p = p->tower[level]; // "next" node has smaller value to the searched one, so the search one can be in front of (somewhere) the "next" node, because the elements in the list are sorted in increasing order.
			}
			else // Found the element.
			{
				break;
			}
		}

		return jumps;
	}
public:
	// Inserts a new value in the list.
	void insert(const T& val)
	{
		if (!root) // It will be the first element in the list.
		{
			(root = new Node(val, maxTowerHeight))->setTowerToNulls();
			memoryTaken += sizeof(Node) + sizeof(Node*) * root->count; // The size of the created node.
			towerHeightsStatistics[root->count]++;
		}
		else
		{
			Node * n = new Node(val, generateTowerHeight()); // Creating the node and it's tower, so I can use the tower to keep the nodes from the path I have gone through. (if I need them)
			n->setTowerToNulls(); // It is possible not to fill the the tower with the nodes I have gone through(for example, I need to insert after the last node but it has not gone through last tower's level).
			(val <= root->val) ? insertFront(n) : insertAfterFront(n); // If the val is smaller than the root's val, I need to put it before the root node, otherwise somewhere after the root node.
			memoryTaken += sizeof(Node) + sizeof(Node*) * n->count; // The size of the created node.
			towerHeightsStatistics[n->count]++;
		}

	//	assert(checkIfSort());
	}

	// Removes the node with @val value.
	void remove(const T& val)
	{
		if (!root)
			return;

		if (root->val == val)
			removeFront();
		else
			removeAfterFront(val);

	//	assert(checkIfSort());
	}

	// Returns true if the searched value is in the list, otherwise returns false.
	bool search(const T& val) const
	{
		return search(val, root);
	}

	// Prints the values of the nodes and the height of the towers.
	void print() const
	{
		if (!root)
			return;

		Node *curr = root, *next = root->tower[0];
		while (next)
		{
			cout << curr->val << " and tower height " << curr->count << "\n";
			curr = next;
			next = next->tower[0];
		}
		cout << curr->val << " and tower height " << curr->count << "\n";
	}
private:
	// Removes the root node.
	void removeFront()
	{
		if (!root || root->tower[0] == NULL) // There is no other element. (!root is not necessary, but doesn't hurt to be there)
		{
			memoryTaken -= sizeof(Node) + sizeof(Node*) * root->count;
			towerHeightsStatistics[root->count]--;
			delete root;
			root = NULL;
		}
		else // There are at least two nodes in the list.
		{
			Node * secondNode = root->tower[0];
			memoryTaken -= sizeof(Node) + sizeof(Node*) * secondNode->count;
			towerHeightsStatistics[secondNode->count]--;
			// I need to delete the root element, so I need to be sure that the second node has tower with maximal height.
			// Fixing the towers.
			memcpy(root->tower, secondNode->tower, secondNode->count * sizeof(Node*)); // Copies the tower of second node to the first node's tower, so I can swap it after that.
			// The rest(bigger levels than the second's node tower) remains the same.

			// Fixing the counters.
			secondNode->count = root->count;

			// Swapping the towers.
			std::swap(root->tower, secondNode->tower);

			// Delete the root node and make the second not root.
			Node* toDel = root;
			root = secondNode;
			delete toDel;
		}
	}

	// Removes element somewhere after the first one.
	void removeAfterFront(const T& val)
	{
		Node *p = root, *el = NULL;
		int level = root->count- 1;

		while (level >= 0) // Searching for the element.
		{
			if (!p->tower[level] || p->tower[level]->val > val)
			{
				if (el != NULL)
					cout << "BAAA ko stana tuka ee";
				--level;
			}
			else if (p->tower[level]->val == val) // Found the element.
			{
				el = p->tower[level]; // Keep it(it will rewrite it el->count times, but for now it's OK.
				p->tower[level] = el->tower[level]; // Make it to point not the the searched node, but where the searched node's tower points (at the same @level)
				--level; // continue to search the element one level bellow.
			}
			else
			{
				p = p->tower[level];
			}
		}

		if (el != NULL) // I have found the element.
		{
			memoryTaken -= sizeof(Node) + sizeof(Node*) * el->count;
			towerHeightsStatistics[el->count]--;
		}

		delete el;
	}

	// Inserts the new node at the beginning of the list. (as root node)
	// It's a corner case, I need to insert the new node in front of the first one. I will "swap" their towers.
	// I need the first tower to be with maximal height and if I left the old root with maximal height it will be bad.
	// (The first two nodes will have same tower height, so I can't "jump" over any nodes when I go from the root, because (for example) I will always go the second Node 
	// or it could turn out that all nodes have same tower heights if I inser them in decreasing order)
	void insertFront(Node * n)
	{
		// Fixing the towers.
		memcpy(n->tower, root->tower, n->count * sizeof(Node*)); // Copies the bottom @n->count pointers, so the old root node tower will point the same nodes as previous.
		for (unsigned i = 0; i < n->count; ++i)
			root->tower[i] = root; // Make the new root tower's bottom @n->count elements to point the old root node. (which will be the second node)

		// Fixing the counters.
		root->count = n->count;
		n->count = maxTowerHeight;

		// Swapping the towers.
		std::swap(root->tower, n->tower);

		root = n;
	}

	// Inserts the new element (@n) node into the list (not as first element or infront of the first element!)
	void insertAfterFront(Node * n)
	{
		int level = root->count - 1; // Starting from the top level of the tower.
		Node * p = root;

		// Finding appropriate place for the new node and keep the "path" in the @n's tower.
		while (level >= 0)
		{
			if (!p->tower[level] || p->tower[level]->val >= n->val)
			{
				if (level < n->count) // The levels above @n->count, I can't interrupt them, because the top level of the tower is @n->count - 1
					n->tower[level] = p; 

				--level;
			}
			else
			{
				p = p->tower[level];
			}
		}

		// Now I have in n->tower[0](also @p) the node, after which I have to insert the node @n.
		// In the tower of @n, I have all nodes that I need to restore their connections, because my tower crosses them.
		for (unsigned i = 0; i < n->count; ++i)
		{
			Node * tmp = n->tower[i]; // Get the node
			if (tmp)
			{
				n->tower[i] = tmp->tower[i]; // Look where this node's tower at level @i goes and set's it to the new @n node's tower at level @i
				tmp->tower[i] = n; // Set the interuppted connection to the @n's tower(at level @i).
			}
		}
	}

	// Returns true if the searched value is in the list, otherwise returns false.
	bool search(const T& val, Node * r) const
	{
		if (!r)
			return false;
		if (r->val == val)
			return true;
		
		Node *p = r;
		int level = p->count - 1;

		while (level >= 0)
		{
			if (!p->tower[level] || p->tower[level]->val > val)
			{
				--level; // Search one level down in the same tower.
			}
			else if (p->tower[level]->val < val) 
			{
				p = p->tower[level]; // "next" node has smaller value to the searched one, so the search one can be in front of (somewhere) the "next" node, because the elements in the list are sorted in increasing order.
			}
			else // Found the element.
			{
				return true;
			}
		}

		return false;
	}
private:
	// Generates and returns a tower height.
	unsigned generateTowerHeight() const
	{
		if (maxTowerHeight <= 1)
			return 1;
		unsigned height = 1;
		while (nextFloor() && ++height < maxTowerHeight)
			;
		return height;
	}

	// It's approximately @coinProbability probability, but not exact.
	// There is ~@coinProbability probability to return true. (and ~(1 - @coinProbability) to return false)
	bool nextFloor() const
	{
		return (rand() % 100) < coinProbability;
	}

	// Frees the allocated memory iterative, recurtion will most likely to overflow.
	void free(Node * root)
	{
		if (!root)
			return;

		Node *curr = root, *next = root->tower[0];
		while (next)
		{
			delete curr;
			curr = next;
			next = next->tower[0];
		}
		delete curr;
	}

	// Returns true if all elements in the list are in increasing order, otherwise returns false.
	bool checkIfSort() const
	{
		if (!root)
			return true;

		Node *curr = root, *next = root->tower[0];
		while (next)
		{
			if (curr->val > next->val)
				return false;

			curr = next;
			next = next->tower[0];
		}

		return true;
	}

	void createTowerHeightStatistics()
	{
		towerHeightsStatistics = new (std::nothrow) unsigned[maxTowerHeight + 1]; // TODO check NULL...
		memset(towerHeightsStatistics, 0, (maxTowerHeight + 1) * sizeof(unsigned));
	}
private:
	// "Ban" copy constructor and operator=.
	SkipList(const SkipList& o);
	SkipList& operator=(const SkipList& o);
};

#endif
