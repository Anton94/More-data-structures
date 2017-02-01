/*
*	Anton Vasilev Dudov
*	Github repository: https://github.com/Anton94/More-data-structures/tree/master/Tree%20isomorphism/Tree%20isomorphism
*
*
*	Presentation I looked - http://logic.pdmi.ras.ru/~smal/files/smal_jass08_slides.pdf
*/

#include <iostream>
#include <string>
#include "Tree.h"

void test(const char * l, const char * r, bool expected)
{
	Tree t1, t2;
	t1.buildTree(l);
	t2.buildTree(r);
	std::cout << "Test ... " << (t1.isIsomorphic(t2) == expected ? "passed" : "FAILED") << "!\n";
}

// Runs some basic tests.
void tests()
{
	// Memory leaks cheching...
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);

	{
		test("", "", true);
		test("(1 {})", "", false);
		test("", "(1 {})", false);
		test("(2 {})", "(1 {})", true);

		test("(5 {(9 {(3 {})}) (1 {(4 {}) (12 {}) (42 {(3 {})})})})",
			"(5 {(9 {(3 {})}) (1 {(4 {}) (12 {}) (42 {(3 {})})})})", true);

		test("(5 {(9 {(3 {})}) (1 {(4 {}) (12 {}) (42 {(3 {})})})})",
			"(5 {(9 {(3 {})}) (1 {(4 {}) (12 {}) (42 {})}}))", false);
		test("(5 {(9 {(3 {})}) (1 {(4 {}) (12 {}) (42 {})}}))",
			"(5 {(9 {(3 {})}) (1 {(4 {}) (12 {}) (42 {(3 {})})})})", false);

		test("(5 {(9 {}) (1 {(4 {}) (12 {}) (42 {})})})",
			"(5 {(1 {(4 {}) (12 {}) (42 {})}) (9 {})})", true);

		test("(5 {(9 {}) (1 {(4 {}) (12 {}) (42 {})})})",
			"(5 {(1 {(4 {}) }) (9 {(12 {})})})", false);

		test("(5 {(9 {(1 {})})})",
			"(5 {(1 {}) (2 {})})", false);

		// Same number nodes, same number nodes at levels, but one parent with two childs vs 2 parents with one child.
		test("(5 {(51 {(511 {}) (512 {})}) (52 {}) (53 {}) (54 {}) (55 {}) (56 {}) (57 {}) (58 {})})",
			"(5 {(51 {(511 {})}) (52 {}) (53 {}) (54 {}) (55 {}) (56 {}) (57 {}) (58 {(581 {})} })", false);
		test("(5 {(51 {(511 {}) (512 {})}) (52 {}) (53 {}) (54 {}) (55 {}) (56 {}) (57 {}) (58 {})})",
			"(5 {(51 {}) (52 {}) (53 {}) (54 {}) (55 {}) (56 {}) (57 {}) (58 {(581 {}) (582 {})})})", true);

		// Same number of leafes but not isomorphic
		test("(5 {(51 {}) (52 {(521 {}) (522 {(5221 {(52211 {(522111 {})})})})})})",
			"(5 {(51 {(511 {(5111 {(51111 {})})})}) (52 {(521 {}) (522 {})})})", false);

		// Same level-by-level spectrum degree but not isomorphic.
		test("(5 {(51 {(511 {}) (512 {})}) (52 {(521 {(5211 {}) (5212 {})})})})",
			"(5 {(51 {(511 {}) (512 {(5121 {}) (5122 {})})}) (52 {(521 {})})})", false);
	}


	_CrtMemCheckpoint(&s2);

	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		std::cout << "Memory leak detected!" << std::endl;
		_CrtMemDumpStatistics(&s3);
	}	
}

// Reads two strings from the input and checks if the trees they represent are isomorphic, if so- prints YES, otherwise prints NO
void testFromInput()
{
	std::string l, r; // Not the best way to do the reading, but it's OK enough for that task.
	std::getline(std::cin, l);
	std::getline(std::cin, r);

	Tree t1, t2;
	t1.buildTree(l.c_str());
	t2.buildTree(r.c_str());

	std::cout << (t1.isIsomorphic(t2) ? "YES" : "NO") << "\n";
}

int main()
{
	testFromInput();
	
	return 0;
}