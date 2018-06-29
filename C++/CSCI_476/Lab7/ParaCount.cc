/*
	FileName	: ParaMerge.cc
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Lab 7
	Description	: Parallel Counting Sort
*/

/************************************************************/
// System includes

#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <thread>
#include <functional>
#include <atomic>

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
using std::atomic_uint;

/************************************************************/
// Function prototypes/global vars/typedefs

typedef std::vector<unsigned>::iterator Iterator;

vector<unsigned> 
createVector( unsigned N , unsigned k );

void
printVector( vector<unsigned> vec );

void
serial_counting_sort ( vector<unsigned>& vec, unsigned p, unsigned k );

void
parallel_counting_sort ( vector<unsigned>& vec, unsigned cutOff, unsigned p, unsigned k );

void
counting_helper ( Iterator begin, Iterator end, atomic_uint* k_count );

/************************************************************/

int
main(int argc, char* argv[])
{
	cout << boolalpha << fixed;
	cout.precision( 3 );
	cout << endl << "N      ==> ";
	unsigned N;
	cin >> N;

	cout << "k      ==> ";
	unsigned k;
	cin >> k;

	cout << "p      ==> ";
	unsigned p;
	cin >> p;

	cout << "Cutoff ==> ";
	unsigned cutOff;
	cin >> cutOff;
	cout << endl;

	vector<unsigned> A = createVector( N , k );
	vector<unsigned> B = A;
	vector<unsigned> C = A;

	printVector( A );
	printVector( B );
	printVector( C );

	Timer< > t;
	parallel_counting_sort( A, cutOff, p, k );
	t.stop();
	cout << "Parallel time:  " << t.getElapsedMs() << " ms" << endl;

	t.start();
	serial_counting_sort ( B, p, k );
	t.stop();
	cout << "Serial time:    " << t.getElapsedMs() << " ms" << endl;

	t.start();
	sort( C.begin() , C.end() );
	t.stop();
	cout << "std::sort time: " << t.getElapsedMs() << " ms" << endl << endl;
	cout << "Parallel versions ok? " << ( A == C ) << endl;
	cout << "Serial versions ok?   " << ( B == C ) << endl << endl;

	printVector( A );
	printVector( B );
	printVector( C );

	return EXIT_SUCCESS;
}

/************************************************************/

vector<unsigned> 
createVector( unsigned N , unsigned k )
{
	vector<unsigned> vec( N );
	random_device rd;
    mt19937 gen( rd() );
  	generate_n ( vec.begin() , N , [&] () { return gen() % k; } );
	return vec;
}

/************************************************************/

void
printVector( vector<unsigned> vec )
{
	cout << "[";
	for ( auto i : vec ) cout << " " << i << " ";
	cout << "]" << endl << endl;
}

/************************************************************/

void
serial_counting_sort ( vector<unsigned>& vec, unsigned p, unsigned k )
{
	vector<unsigned> k_count( k , 0 );
	for ( unsigned i = 0; i < vec.size(); ++i )
	{
		++( k_count[ vec[ i ]]);
	}

	unsigned pos = 0;
	for ( unsigned i = 0; i < k; ++i )
	{
		if ( k_count[ i ] > 0 )
		{
			for ( unsigned j = 0; j <= k_count[ i ]; ++j )
			{
				vec[ pos ] = i;
				++pos;
			}
		}
	}
}

/************************************************************/

void
parallel_counting_sort ( vector<unsigned>& vec, unsigned cutOff, unsigned p, unsigned k )
{
	if ( vec.size() <= cutOff )
	{
		serial_counting_sort( vec, p, k );
	}
	else
	{
		vector<thread> threads( p );
		atomic_uint* k_count = new atomic_uint[ k ];
		for ( unsigned i = 0; i < k; ++i )
		{
			k_count[ i ] = 0;
		}

		unsigned section_size = vec.size() / p;

		for ( unsigned i = 0; i < p; ++i )
		{
			unsigned section = section_size * i;
			unsigned section_end = section + section_size;

			threads[ i ] = thread ( counting_helper, vec.begin() + section, 
				vec.begin() + section_end, k_count );
		}
		for ( thread& t : threads ) t.join();

		unsigned pos = 0;
		for ( unsigned i = 0; i < k; ++i )
		{
			if ( k_count[ i ] > 0 )
			{
				for ( unsigned j = 0; j <= k_count[ i ]; ++j )
				{
					vec[ pos ] = i;
					++pos;
				}
			}
		}
		delete [] k_count;
	}
}

/************************************************************/

void
counting_helper ( Iterator begin, Iterator end, atomic_uint* k_count )
{
	for ( unsigned i = 0; i < end - begin; ++i )
	{
		++k_count[ *( begin + i ) ];
	}
}

/************************************************************/