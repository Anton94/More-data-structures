/*
*
*
*	TODO & NOTES:
*		- TODO: simpler structures for node's childrens.
*		- TODO: Memory checking when dynamically allocated.
*		- I expect that the number of childrens to be not so big number(even if it was big), so it will be faster to do the operations with node's name with array(finding the node's spectrum name,sort the childs spectrum names).
*		- TODO: iterative building of the tree.
*		- NOTE: I keep the whole BFS traversal of the trees in memory to be easier to iterate the levels from the bottom to top, but for bigger trees it may cause problems.
*/

#ifndef TREE__
#define TREE__

#include <list>
#include <algorithm> 
using std::list;
using std::sort;

class Tree
{
	struct Node
	{
		friend class Comparator;
		list<Node*> childs;
		unsigned * name; // Here is the spectrum degree's name of the subtree(which is spectrum degree names of it's childs, sorted in array(so the child order will not matter)) and if it's processed it will be just some sort of "index name" at first entry of the array.
		unsigned count;

		Node() { name = NULL; count = 0; }
		~Node() { delete[] name; }

		// Creates the name of a node(spectrum degree's name). Get's the names (which are unsigned values) of it's childs and keeps them in sorted array.
		// It's expected to be called once per node.
		void generateName();

		// Set's only the given value for the spectrum degree's name of that node.
		void setName(unsigned n);

		// Compares the names values of the two nodes.
		bool operator<(const Node& other) const;
		bool operator==(const Node& other) const;
		bool operator!=(const Node& other) const {	return !operator==(other);	}
	private:
		// Sorts the names of node's childs.
		void sortName()	{	sort(name, name + count); }
	};

	// Used by the sort function. Compares two nodes by their name values.
	struct Comparator
	{
		bool operator() (Node* l, Node* r) { return (*l < *r); }
	} nodeComparator;
private:
	Node * root;
	unsigned numberOfLevels;
	unsigned numberOfElements;
public:
	Tree() { setDefaultValues(); }
	~Tree() { free(); }
	unsigned getNumberOfLevels() const { return numberOfLevels; }
	unsigned getNumberOfElements() const { return numberOfElements; }

	// Builds the tree.
	void buildTree(const char * in);

	// Checks if the trees are isomorphic.
	bool isIsomorphic(Tree& other);
private:
	// Checks if the given chuncks of the @l and @r arrays(which represents a level of the trees) have same spectrum degree names. 
	bool checkLevelMatches(unsigned start, unsigned end, const Node*const* l, const Node *const*r) const;

	// Makes BFS traversal and keeps the nodes into the buffer array.
	void BFSTraversal(Node ** buff) const;

	// Recursive generating of the tree.
	void generateTreeByString(const char*& data, Node *& n, unsigned level);
private:
	// Sets the default values.
	void setDefaultValues();

	// Iterative freeing of the tree.
	void free();
private:
	// Ban copy constructor and operator=
	Tree(const Tree& other);
	Tree& operator=(const Tree& other);
};


#endif