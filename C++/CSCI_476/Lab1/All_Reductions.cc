/*
	FileName	: All_Reductions.cc
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Lab 1 - Extra Credit 1
	Description	: Shows the communication and computation pattern 
					for the tree-based parallel one-to-all broadcast.
*/

/************************************************************/
// System includes

#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::uniform_int_distribution;
using std::random_device;
using std::mt19937;

/************************************************************/
// Function prototypes

vector<int> createVector( int p );

int isPowerOfTwo( int x );

int powerOfTwo( int x );

int findPosition( size_t pos, size_t size);

void sumVector( vector<int> &vec );

void printSummary( size_t stages , size_t adds , vector<int> &start , vector<int> &end );

/************************************************************/

int
main(int argc, char* argv[])
{
	int p = 0;
	while ( !isPowerOfTwo( p ) )
	{
		cout << "p ==> ";
		cin >> p;
		cout << endl;
	}
	vector<int> randVector = createVector( p );
	sumVector ( randVector );

	return EXIT_SUCCESS;
}

/************************************************************/

int isPowerOfTwo( int x )
{
	if ( x < 1 )
	{
		return 0;
	}

	while ( x % 2 == 0 )
	{
		x >>= 1;
	}
	return ( x == 1 );
}

/************************************************************/

// Assumes number is a power of 2
int powerOfTwo( int x )
{
	int power = 0;
	while ( x % 2 == 0 )
	{
		x >>= 1;
		++power;
	}
	return power;
}

/************************************************************/

int findPosition( size_t pos, size_t size)
{
	return pos % size;
}

/************************************************************/

vector<int> createVector( int p )
{
	vector<int> vec( p );

	// Source: http://www.cplusplus.com/reference/random/uniform_int_distribution/operator()/
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	std::default_random_engine gen ( seed );
  	uniform_int_distribution<int> dist( 10, 99 );

	for ( auto &num : vec )
	{
		num = dist( gen );
	}

	return vec;
}

/************************************************************/

void sumVector( vector<int> &vec )
{
	size_t stages = 0;
	size_t adds = 0;
	size_t halfSize = vec.size() >> 1;
	vector<int> start( vec );

	for ( size_t mod = 1; mod < vec.size(); mod <<= 1 )
	{
		cout << "Stage " << stages << endl;
		cout << "-------" << endl;

		for ( size_t i = 0; i < halfSize; ++i )
		{
			// pos has an index of the second part of the array
			int pos = findPosition ( i + mod , halfSize ) + halfSize;
			int sum = vec[ i ] + vec[ pos ];
			
			cout << "Recv: " << i << " from " << pos << ", v = " << vec[ pos ]
				<< ", sum = " << sum << endl << endl;

			cout << "Recv: " << pos << " from " << i << ", v = " << vec[ i ]
				<< ", sum = " << sum << endl << endl;

			// Exchange which would mean 2 adds in parallel version
			vec[ i ] = sum;
			vec[ pos ] = sum;
			adds += 2;
		}
		++stages;
	}
	printSummary( stages , adds , start , vec );
}

/************************************************************/

void printSummary( size_t stages , size_t adds , vector<int> &start , vector<int> &end )
{
	cout << "Summary" << endl;
	cout << "=======" << endl;

	cout << "A[] = { " << start[0];
	for ( size_t i = 1; i < start.size(); ++i )
	{
		cout << ", " << start[i];
	}
	cout << " }" << endl;

	cout << "S[] = { " << end[0];
	for ( size_t i = 1; i < end.size(); ++i )
	{
		cout << ", " << end[i];
	}
	cout << " }" << endl;

	cout << "# stages  = " << stages << endl;
	cout << "# adds    = " << adds << endl;
}

/************************************************************/
