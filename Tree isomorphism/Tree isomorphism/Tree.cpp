#include <stack>
#include "Tree.h"
using std::stack;
using std::min;

/*
*
*	Node
*
*/

// Creates the name of a node(spectrum degree's name). Get's the names (which are unsigned values) of it's childs and keeps them in sorted array.
// It's expected to be called once per node.
void Tree::Node::generateName()
{
	if (childs.empty()) // A leaf
	{
		setName(0);
	}
	else
	{
		delete[] name;
		count = childs.size();
		name = new (std::nothrow) unsigned[count];
		// Get the spectrum degrees of it's childs(which are already processed and they have some sort of index value for their names at the first possition of the array(for a little memory optimization I reuse it..)
		unsigned i = 0;
		for (list<Node*>::iterator it = childs.begin(), end = childs.end(); it != end; ++it, ++i)
		{
			name[i] = (*it)->name[0];
		}

		sortName(); // Sort it.
	}
}

// Set's only the given value for the spectrum degree's name of that node.
void Tree::Node::setName(unsigned n)
{
	delete[] name;
	name = new (std::nothrow) unsigned[1]; // Not very good, but..
	name[0] = n;
	count = 1;
}

// Compares the names values of the two nodes.
bool Tree::Node::operator<(const Node& other) const
{
	unsigned i = 0;
	for (unsigned bound = min(count, other.count); i < bound; ++i)
		if (name[i] < other.name[i]) // LHS is smaller
			return true;
		else if (name[i] > other.name[i]) // LHS is bigger
			return false;

	// The whole LHS(or RHS) name's matches
	if (count < other.count) // The elements in LHS are less than the elements in RHS and all
		return true;
	else // They are equal or the elements in LHS are more thant the elements in RHS
		return false;
}

// Compares the names values of the two nodes.
bool Tree::Node::operator==(const Node& other) const
{
	if (count != other.count)
		return false;

	for (unsigned i = 0; i < count; ++i)
		if (name[i] != other.name[i])
			return false;

	return true;
}

/* 
* 
*	Tree
* 
*/

// Builds the tree.
void Tree::buildTree(const char * in)
{
	free();
	setDefaultValues();
	generateTreeByString(in, root, 1);
}

// Checks if the trees are isomorphic.
/* First, makes BFS traversal and keeps the nodes(pointers to them) into array, seperated (level-by-level) by NULL pointers.
Starting from the bottom level of the tree:
- generates the spectrum names for each node of the level(array of unsigneds) and sorts them.
- sorts the nodes from this level by their names.
- checks if the two(of the two trees) sorted levels(the level is array of pointers to nodes) have the same spectrum names(the corresponding(at the same index in the arrays) nodes have the same spectrum names)
- gives new names for the nodes, only one unsigned value for each node
*/
bool Tree::isIsomorphic(Tree& other)
{
	if (getNumberOfElements() != other.getNumberOfElements() || getNumberOfLevels() != other.getNumberOfLevels())
		return false;
	bool status = true;
	unsigned currentLevelEnd, // The index of NULL ptr after the last node from the current level.
		size = getNumberOfElements() + getNumberOfLevels() + 1; // For the first NULL ptr in the array(before the root).

	Node **firstBFS = new (std::nothrow) Node*[size]; // For each element and each level is seperated by NULL (+1 because I start with NULL seperator)
	Node **secondBFS = new (std::nothrow) Node*[size];
	BFSTraversal(firstBFS);
	other.BFSTraversal(secondBFS);

	for (unsigned i = size - 1, currentLevelEnd = size - 1; i-- > 0; ) // First  --i will skip the last NULL ptr, which is OK.
	{
		if ((!firstBFS[i] && secondBFS[i]) || (firstBFS && !secondBFS)) // In one of the trees the nodes at the current level are processed, but in the other there are more.
		{
			status = false; // The number of nodes at this level are different.
			break;
		}

		if (!firstBFS[i] && !secondBFS[i]) // The nodes from this level are processed(have generated their names) and @i is at the seperator of next level.
		{
			// Sort the nodes at this level 
			sort(firstBFS + i + 1, firstBFS + currentLevelEnd, nodeComparator);
			sort(secondBFS + i + 1, secondBFS + currentLevelEnd, nodeComparator);

			// Check if the level's nodes have same spectrum degree names.
			if (!(status = checkLevelMatches(i + 1, currentLevelEnd, firstBFS, secondBFS)))
				break;

			// Give new (one value) names to the nodes in the level. (If the old names matches, the new once must match too)
			for (unsigned j = i + 1 + 1, id = 0; j < currentLevelEnd; ++j) // j starts from the second node in the level
			{
				firstBFS[j - 1]->setName(id);
				secondBFS[j - 1]->setName(id);
				if (*firstBFS[j - 1] == *firstBFS[j]) // if the next node has same spectrum degree name. (same as *secondBFS[j - 1] == *secondBFS[j], because I already know that the levels match)
					++id;
			}

			currentLevelEnd = i; // Pointing one element after the last node from previous level. (points to NULL ptr)
		}
		else // firstBFS[i] && secondBFS[i] (both are valid nodes)
		{
			firstBFS[i]->generateName();
			secondBFS[i]->generateName();
		}
	}

	delete[] firstBFS;
	delete[] secondBFS;

	return status;
}

// Checks if the given chuncks of the @l and @r arrays(which represents a level of the trees) have same spectrum degree names. 
bool Tree::checkLevelMatches(unsigned start, unsigned end, const Node*const* l, const Node *const*r) const
{
	for (unsigned j = start; j < end; ++j)
		if (*l[j] != *r[j])
			return false;

	return true;
}

// Makes BFS traversal and keeps the nodes into the buffer array.
void Tree::BFSTraversal(Node ** buff) const
{
	if (!root)
		return;
	Node * curr = root;
	buff[0] = NULL;
	buff[1] = root;
	buff[2] = NULL;
	for (unsigned i = 3, q = 1, bound = getNumberOfElements() + getNumberOfLevels(); i < bound; ) // @i represents the whole array counter, and @q the partial "queue"(iterated level someway..)
	{
		if (buff[q] == NULL)
			buff[i++] = NULL;
		else
			for (list<Node*>::iterator it = buff[q]->childs.begin(), end = buff[q]->childs.end(); it != end; ++it)
				buff[i++] = (*it);
		++q;
	}
}

// Recursive generating of the tree.
void Tree::generateTreeByString(const char*& data, Node *& n, unsigned level)
{
	if (*data != '(')
		return;
	++data; // skip the '('

	if (*data == '-') // Skip the number sign
		++data;
	while (*data >= '0' && *data <= '9') // Skip the number
		++data;
	if (*data == ' ') // Skip the white space.
		++data;

	n = new Node();
	++numberOfElements; // Keep track of the number of elements and levels.
	if (level > numberOfLevels)
		numberOfLevels = level;

	if (*data != '{')
		return;
	++data; // skip the '{'

	while (*data != ')')
	{
		Node * temp = NULL;

		if (*data == '(')
		{
			generateTreeByString(data, temp, level + 1);
			n->childs.push_back(temp);
		}
		else // *data == '}' No children
		{
			++data;
		}
	}
	++data; // Skip the ')'

	if (*data == ' ') // Skip the white space.
		++data;
}

// Sets the default values.
void Tree::setDefaultValues()
{
	root = NULL;
	numberOfLevels = 0;
	numberOfElements = 0;
}

// Iterative freeing of the tree.
void Tree::free()
{
	if (!root)
		return;

	stack<Node*> path; // Always only not NULL nodes on the path.
	path.push(root);

	Node * curr;

	while (!path.empty())
	{
		curr = path.top();
		path.pop();

		// Iterate over it's childs and if they are not NULL, push them into the stack.
		for (list<Node*>::iterator it = curr->childs.begin(), end = curr->childs.end(); it != end; ++it)
			if ((*it))
				path.push((*it));

		delete curr;
	}
}