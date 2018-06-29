/*
  Filename   : Josephus.cpp
  Author     : Zachary Zuch
  Course     : CSCI 362
  Assignment : Lab 5
  Description: This program takes a list of integers and 
  				removes the kth element and then the kth 
  				element after that in a circle; then, the 
  				program prints the elements removed and the 
  				remaining element.
*/

/************************************************************/
// System includes

#include <cstdlib>
#include <iostream>

/************************************************************/
// Local includes

#include "List.hpp"

/************************************************************/
// Using declarations

using std::cin;
using std::cout;
using std::endl;

/************************************************************/
// Function prototypes

template<typename T>
void
execute ( List<T> &L, size_t execNum );

void
listFill ( List<int> &L , int numPeople );

/************************************************************/

int main(int argc, char* argv[])
{
	int numPeople;
	cout << "N ==> ";
	cin >> numPeople;

	List<int> L;
	listFill ( L , numPeople );

	size_t execNum;
	cout << "k ==> ";
	cin >> execNum;
	cout << endl;

	execute ( L , execNum );

	return EXIT_SUCCESS;
}

/************************************************************/

// Complexity:		T( N, k ) = (N - 1) * k
template<typename T>
void
execute ( List<T> &L, size_t execNum )
{
	typename List<T>::iterator curr = L.begin();
	cout << "Execution order: ";
	while ( L.size() > 1 )
	{
		if ( curr == L.end() ) { ++curr; }

		for ( size_t i = 1; i < execNum; ++i ) 
		{ 
			++curr; 
			if ( curr == L.end() ) { ++curr; }
		}

		cout << *( curr ) << " ";
		curr = L.erase ( curr );
	}
	cout << endl;
	cout << "Survivor is " << L.front() << endl;
}

/************************************************************/

void
listFill ( List<int> &L , int numPeople )
{
	for ( int i = numPeople; i > 0 ; --i )
	{
		L.push_front( i );
	}
}

/************************************************************/
