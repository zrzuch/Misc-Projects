/*
  Filename   : Josephus.cpp
  Author     : Zachary Zuch
  Course     : CSCI 362
  Assignment : Lab 4
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
#include <list>

/************************************************************/
// Using declarations

using std::cin;
using std::cout;
using std::endl;
using std::list;

/************************************************************/
// Function prototypes

template<typename T>
void
execute ( list<T> &L, size_t execNum );

void
listFill ( list<int> &L , int numPeople );

/************************************************************/

int main(int argc, char* argv[])
{
	int numPeople;
	cout << "N ==> ";
	cin >> numPeople;

	list<int> L;
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
execute ( list<T> &L, size_t execNum )
{
	typename std::list<T>::iterator start = L.begin();
	size_t curr = 1;
	cout << "Execution order: ";
	while ( L.size() > 1 )
	{
		if ( curr == execNum )
		{
			cout << (*start) << " ";
			if ( start == L.end() )
			{
				L.pop_back();
				start = L.begin();
			}
			else
			{
				start = L.erase ( start );
			}
			curr = 0;
		}
		else
		{
			if ( start == L.end() )
			{
				start = L.begin();
			}
			else 
			{
				++start;
			}
			++curr;
		}
		
	}
	cout << endl;
	cout << "Survivor is " << L.front() << endl;
}

/************************************************************/

void
listFill ( list<int> &L , int numPeople )
{
	for (int i = numPeople; i > 0 ; --i)
	{
		L.push_front( i );
	}
}

/************************************************************/
