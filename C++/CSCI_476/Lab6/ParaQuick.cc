/*
	FileName	: ParaMerge.cc
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Lab 6
	Description	: Parallel Quick Sort
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
using std::iter_swap;
using std::random_device;

/************************************************************/
// Function prototypes/global vars/typedefs

vector<int> 
createVector( size_t N );

void
printVector( vector<int> vec );

template <class RAI>
void
insertion_sort( RAI first, RAI last );

template <class RAI>
void 
serial_merge_sort( RAI first, RAI last, unsigned cutOff );

template <class RAI>
void 
parallel_merge_sort( RAI first, RAI last, unsigned cutOff, int depth );

template <typename RAI>
void
serial_quick_sort ( RAI first, RAI last, unsigned cutOff );

template <typename RAI>
void
parallel_quick_sort ( RAI first, RAI last, unsigned cutOff, int depth );

template <typename RAI>
RAI
partition ( RAI first, RAI last );

template <typename RAI>
RAI
median_3 ( RAI first, RAI last );

void 
printSummary( double parallelMergeTime, double serialMergeTime, double parallelQuickTime, 
				double serialQuickTime, double sortTime, bool parallelCheck, bool serialCheck );

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
	vector<int> D = A;
	vector<int> E = A;

	Timer< > t;
	parallel_merge_sort( A.begin(), A.end(), cutOff, depth );
	t.stop();
	double parallelMergeTime = t.getElapsedMs();

	t.start();
	serial_merge_sort( B.begin(), B.end(), cutOff );
	t.stop();
	double serialMergeTime = t.getElapsedMs();

	t.start();
	parallel_quick_sort( C.begin(), C.end(), cutOff, depth );
	t.stop();
	double parallelQuickTime = t.getElapsedMs();

	t.start();
	serial_quick_sort( D.begin(), D.end(), cutOff );
	t.stop();
	double serialQuickTime = t.getElapsedMs();

	t.start();
	sort( E.begin() , E.end() );
	t.stop();
	double sortTime = t.getElapsedMs();

	printVector( A );
	printVector( B );
	printVector( C );
	printVector( D );
	printVector( E );

	printSummary( parallelMergeTime, serialMergeTime, parallelQuickTime, serialQuickTime, 
		sortTime, ( A == E ) && ( C == E ), ( B == E ) && ( D == E ) );
	return EXIT_SUCCESS;
}

/************************************************************/

vector<int> 
createVector( size_t N )
{
	vector<int> vec( N );
	random_device rd;
    mt19937 gen( rd() );
  	generate_n ( vec.begin() , N , [&] () { return gen() % 1000; } );
	return vec;
}

/************************************************************/

void
printVector( vector<int> vec )
{
	cout << "[";
	for ( auto i : vec ) cout << " " << i << " ";
	cout << "]" << endl << endl;
}

/************************************************************/

template <class RAI>
void
insertion_sort( RAI first, RAI last )
{
	for ( auto i = first + 1; i < last; ++i )
	{
		for ( auto j = i; j > first && *j < *( j - 1 ); --j ) iter_swap( j , j - 1 );
	}
}

/************************************************************/

template <class RAI>
void
serial_merge_sort( RAI first, RAI last, unsigned cutOff )
{
    if ( last - first > cutOff )
    {
    	RAI middle = first + ( last - first ) / 2;
    	serial_merge_sort( first , middle , cutOff );
    	serial_merge_sort( middle , last , cutOff );
    	inplace_merge( first , middle , last );
    }
    else insertion_sort( first , last );
}

/************************************************************/

template <class RAI>
void 
parallel_merge_sort( RAI first, RAI last, unsigned cutOff, int depth )
{
    if ( last - first > cutOff )
    {
    	if ( depth == 0 )
    	{
    		serial_merge_sort( first , last , cutOff );
    	}
    	else
    	{
    		RAI middle = first + ( last - first ) / 2;
        	thread t( parallel_merge_sort<RAI> , first , middle , cutOff , depth - 1 );
        	parallel_merge_sort( middle , last , cutOff , depth - 1 );
        	t.join();
        	inplace_merge( first , middle , last );
    	}
    }
    else insertion_sort( first , last );
}

/************************************************************/

template <typename RAI>
void
serial_quick_sort ( RAI first, RAI last, unsigned cutOff )
{
	if ( first + cutOff < last )
	{
		RAI pivot = partition( first , last );
		serial_quick_sort( first, pivot, cutOff );
		serial_quick_sort( pivot + 1, last, cutOff );
	}
	else insertion_sort( first , last );
}

/************************************************************/

template <typename RAI>
void
parallel_quick_sort ( RAI first, RAI last, unsigned cutOff, int depth )
{
	if ( first + cutOff < last )
	{
		if ( depth == 0 )
		{
			serial_quick_sort( first , last , cutOff );
		}
		else
		{
			RAI pivot = partition( first , last );
			thread t( parallel_quick_sort<RAI> , first , pivot , cutOff , depth - 1 );
			parallel_quick_sort( pivot + 1, last, cutOff, depth - 1 );
			t.join();
		}
	}
	else insertion_sort( first , last );
}

/************************************************************/

template <typename RAI>
RAI
partition ( RAI first, RAI last )
{
	RAI pivot = median_3( first , last - 1 ) , up = first , down = last - 2;
	while ( true )
	{
		while ( *( ++up ) < *pivot ) { }
		while ( *( --down ) > *pivot ) { }
		if ( up - down >= 0 ) break;
		iter_swap( up , down );
	}
	iter_swap( last - 2 , up );
	return up;
}

/************************************************************/

template <typename RAI>
RAI
median_3 ( RAI first, RAI last )
{
	RAI middle = first + ( last - first ) / 2;
	if ( *first > *middle ) iter_swap( first , middle );
	if ( *first > *last ) iter_swap( first , middle );
	if ( *middle > *last ) iter_swap( middle , last );
	iter_swap( middle , last - 1 );
	return ( last - 1 );
}

/************************************************************/

void 
printSummary( double parallelMergeTime, double serialMergeTime, double parallelQuickTime, 
				double serialQuickTime, double sortTime, bool parallelCheck, bool serialCheck )
{
	cout << boolalpha << fixed;
	cout.precision( 3 );
	cout << "Parallel MS time:  " << parallelMergeTime << " ms" << endl;
	cout << "Serial MS time:    " << serialMergeTime << " ms" << endl << endl;
	cout << "Parallel QS time:  " << parallelQuickTime << " ms" << endl;
	cout << "Serial QS time:    " << serialQuickTime << " ms" << endl << endl;
	cout << "std::sort time:    " << sortTime << " ms" << endl << endl;
	cout << "Parallel versions ok? " << parallelCheck << endl;
	cout << "Serial versions ok?   " << serialCheck << endl << endl;
}

/************************************************************/