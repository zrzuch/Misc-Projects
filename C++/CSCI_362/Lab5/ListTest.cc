/*
  Filename   : Josephus.cpp
  Author     : Zachary Zuch
  Course     : CSCI 362
  Assignment : Lab 5
  Description: This program tests all of the functions in my
  				List class.
*/

/************************************************************/
// System includes

#include <iostream>
#include <cstdlib>

/************************************************************/
// Local includes

#include "List.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::endl;

/************************************************************/

int 
main(int argc, char* argv[])
{
	List<int> L;
	cout << L.size() << endl;
	cout << L.empty() << endl<< endl;

	const List<int> L2( 5 );
	cout << "Size of List #2 = " << L.size() << endl;
	cout << "Front of List #2 = " << L2.front() << endl;
	cout << "Back of List #2 = " << L2.back() << endl << endl;
	cout << "List #2 = " << L2 << endl << endl;
	cout << *( L2.begin() ) << endl;
	cout << *( --L2.end() ) << endl;

	List<int> L4( L2 );
	cout << "List #4 = " << L4 << endl << endl;

	List<int> L3 = { 1 , 2 , 3 , 4 };
	cout << "List #3 = "<< L3 << endl << endl;

	L.push_back(5);
	L.push_front(4);
	cout << L.size() << endl;
	cout << L.empty() << endl << endl;

	cout << "Front of List #1 = "<< L.front() << endl;
	cout << "Back of List #1 = "<< L.back() << endl << endl;
	cout << "List #1 = "<< L << endl << endl;
	
	L.pop_front();
	L.pop_back();
	cout << "List #1 = "<< L << endl << endl;

	cout << L.size() << endl;
	cout << L.empty() << endl << endl;

	L = L2;
	cout << "List #1 = "<< L << endl << endl;
	cout << *( L.begin() ) << endl;
	L.insert( L.end() , 20 );
	L.erase( L.begin() );
	cout << "List #1 = "<< L << endl << endl;
	L.erase( L.begin(), L.end() );
	cout << "List #1 = "<< L << endl << endl;
}

/************************************************************/
