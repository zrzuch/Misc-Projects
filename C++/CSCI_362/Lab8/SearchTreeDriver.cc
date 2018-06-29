/*
  Filename   : SearchTree.hpp
  Author     : Zachary Zuch
  Course     : CSCI 362-01
  Description: This program is the test for the binary 
  				search tree class.
*/

/************************************************************/
// System includes

#include <iostream>
#include <cstdlib>

/************************************************************/
// Local includes

#include "SearchTree.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::endl;

/************************************************************/
// Function prototypes

void printTreeInfo ( SearchTree<int> &tree , int treeNumber );
void createTree1 ( SearchTree<int> &tree );
void createTree2 ( SearchTree<int> &tree );

/************************************************************/

int 
main(int argc, char* argv[])
{
	const static int FIND_NUM = 3;
	SearchTree<int> tree;
	printTreeInfo( tree , 1 );
	createTree1 ( tree );
	printTreeInfo( tree , 1 );
	tree.erase(1); // 0 Children
	printTreeInfo( tree , 1 );
	tree.erase(2); // 1 Child
	printTreeInfo( tree , 1 );
	tree.erase(4); // 2 Children
	printTreeInfo( tree , 1 );
	if ( tree.find(FIND_NUM) != tree.end() )
	{
		cout << *(tree.find(FIND_NUM)) << " was found " << endl;
	}
	else
	{
		cout << FIND_NUM << " was not found" << endl;
	}
	SearchTree<int> tree2;
	createTree2 ( tree2 );
	printTreeInfo ( tree2, 2 );
	cout << endl;
	return EXIT_SUCCESS;
}

/************************************************************/

void createTree1 ( SearchTree<int> &tree )
{
	tree.insert(4);
	tree.insert(2);
	tree.insert(1);
	tree.insert(3);
	tree.insert(6);
	tree.insert(5);
	tree.insert(7);
	
}

void createTree2 ( SearchTree<int> &tree )
{
	for ( int i = 1; i < 4; ++i )
	{
		tree.insert(i);
	}
}

/************************************************************/

void printTreeInfo ( SearchTree<int> &tree , int treeNumber )
{
	
	if ( tree.empty() )
	{
		cout << endl << "SearchTree " << treeNumber << " = ( EMPTY )" << endl;
	}
	else 
	{
		cout << endl << "Search Tree " << treeNumber << ":" <<endl;
		cout << "    Size = " << tree.size() << endl;
		cout << "    Depth = " << tree.depth() << endl;
		cout << "    Level Order Content = " << tree << endl;
		
		cout << "    In Order Content = ( ";
		for ( SearchTree<int>::iterator i = tree.begin(); i != tree.end(); ++i )
		{
			cout << *i << " ";
		}
		cout << ")" << endl;

		cout << "    Reverse Content = ( ";
		for ( SearchTree<int>::iterator i = --tree.end(); i != tree.end(); --i )
		{
			cout << *i << " ";
		}
		cout << ")" << endl;
	}
}

/************************************************************/
