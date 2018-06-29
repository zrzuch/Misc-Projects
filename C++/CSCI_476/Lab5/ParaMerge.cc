/*
	FileName	: ParaMerge.cc
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Lab 5
	Description	: Shows the communication and computation pattern 
					for the tree-based parallel reduction.
*/

/************************************************************/
// System includes

#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <cassert>
#include <thread>

/************************************************************/
// Local includes

#include "Timer.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::mt19937;
using std::generate_n;
using std::sort;
using std::thread;
using std::boolalpha;
using std::inplace_merge;
using std::fixed;

/************************************************************/
// Function prototypes/global vars/typedefs

vector<int> 
createVector( size_t N );

template <class Iterator>
void 
merge_sort( Iterator first , Iterator last , unsigned cutOff , unsigned depth );

void 
printSummary( double parallelTime , double serialTime , double sortTime , 
				bool parallelCheck , bool serialCheck );

/************************************************************/

int
main(int argc, char* argv[])
{
	cout << endl << "N      ==> ";
	int N;
	cin >> N;

	cout << "Cutoff ==> ";
	int cutOff;
	cin >> cutOff;

	cout << "Depth  ==> ";
	int depth;
	cin >> depth;
	cout << endl;

	vector<int> A = createVector( N );
	vector<int> B = A;
	vector<int> C = A;

	Timer< > t;
	merge_sort( A.begin() , A.end() , cutOff , depth );
	t.stop();
	double parallelTime = t.getElapsedMs();

	t.start();
	merge_sort( B.begin() , B.end() , cutOff , 0 );
	t.stop();
	double serialTime = t.getElapsedMs();

	t.start();
	sort( C.begin() , C.end() );
	t.stop();
	double sortTime = t.getElapsedMs();

	printSummary( parallelTime , serialTime , sortTime , 
		( A == C ) , ( B == C ) );

	return EXIT_SUCCESS;
}

/************************************************************/

vector<int> 
createVector( size_t N )
{
	vector<int> vec( N );
    mt19937 gen( 0 );
  	generate_n ( vec.begin() , N , [&] () { return gen() % 1000; } );
	return vec;
}

/************************************************************/

template <class Iterator>
void 
merge_sort( Iterator first , Iterator last , unsigned cutOff , unsigned depth )
{
    if ( last - first > cutOff )
    {
    	Iterator middle = first + ( last - first ) / 2;
    	if ( depth == 0 )
    	{
    		merge_sort( first , middle , cutOff , 0 );
        	merge_sort( middle , last , cutOff , 0 );
    	}
        else
        {
        	thread t( merge_sort<Iterator> , first , middle , cutOff , depth - 1 );
        	merge_sort( middle , last , cutOff , depth - 1 );
        	t.join();
        }
        inplace_merge( first , middle , last );
    }
    else {		sort( first , last ); 		}
}

/************************************************************/

void 
printSummary( double parallelTime , double serialTime , double sortTime , 
				bool parallelCheck , bool serialCheck )
{
	cout << boolalpha << fixed;
	cout.precision( 3 );
	cout << "Parallel time:  " << parallelTime << " ms" << endl;
	cout << "Serial time:    " << serialTime << " ms" << endl;
	cout << "std::sort time: " << sortTime << " ms" << endl << endl;
	cout << "Parallel ok? " << parallelCheck << endl;
	cout << "Serial ok?   " << serialCheck << endl << endl;
}

/************************************************************/