/*
	FileName	: Reductions.cc
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Lab 1
	Description	: Shows the communication and computation pattern 
					for the tree-based parallel reduction.
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

void sumVector( vector<int> &vec );

void printSummary( size_t stages , size_t adds , vector<int> &start , vector<int> &end );

/************************************************************/

int
main(int argc, char* argv[])
{
	int p = 0;

	while ( p < 1 || p > 512 )
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
	vector<int> start( vec );

	for ( size_t mod = 1; mod < vec.size(); mod <<= 1 )
	{
		cout << "Stage " << stages << endl;
		cout << "-------" << endl;

		for ( size_t i = 0; ( i + mod ) < vec.size(); i += mod + mod )
		{
			vec[ i ] += vec[ i + mod ];

			cout << "Recv: " << i << " from " << ( i + mod ) << ", v = " << vec[ i + mod ] 
				<< ", sum = " << vec[ i ] << endl << endl;

			++adds;
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
