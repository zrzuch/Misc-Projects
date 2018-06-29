/*
	FileName	: ThreadSum.cc
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Lab 3
	Description	: Compute a sum in parallel with Threads.
	Process Time: 7.473 ms ( P == 8 && N == 1,000,000 )
	Thread Time : 6.179 ms ( P == 8 && N == 1,000,000 )
	Summary 	: The thread sum seems to be faster because
					it doesn't require the instance to be copied
					each time it was run. Additionally, I updated
					my forking program so that it would just continually
					write to the pipe which caused the program to be 
					faster, but the creation and management of the 
					pipe seems to increase the efficiency greatly. 
					This was shown with my original forking program 
					because it took on average 2.5 times as long 
					to compute the sum. Additionally, when N is 
					extremely low, we can see that both programs
					large overhead compared to the serial sum.
					In a test with p equal to 2 and N equal to 1,
					the forking program time is 0.425952 ms, the
					thread program time is 0.1681 ms, and the
					serial program time is 0.000498 ms which
					obviously shows the differences in overhead
					which demonstrates my points.
*/

/************************************************************/
// System includes

#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <functional>
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
using std::uniform_int_distribution;
using std::random_device;
using std::mt19937;
using std::accumulate;
using std::generate_n;
using std::function;
using std::bind;
using std::thread;
using std::cref;
using std::ref;

/************************************************************/
// Function prototypes/global vars/typedefs

vector<int> 
createVector( size_t N );

int 
threadSum( size_t p );

void
localSum( const vector<int>& A , size_t offset , size_t sections , int& sum );

void 
printSummary( int parallelSum , int serialSum , 
	double parallelTime , double serialTime );

vector<int> A;

/************************************************************/

int
main(int argc, char* argv[])
{
	// # of processes
	size_t p = 0;
	while ( p < 1 )
	{
		cout << "p ==> ";
		cin >> p;
		cout << endl;
	}

	// Size of Vector
	size_t N = 0;
	while ( N < 1 )
	{
		cout << "N ==> ";
		cin >> N;
		cout << endl;
	}

	if ( p > N ) p = N;
	
	A = createVector( N );

	Timer< > t;
	int serialSum = accumulate( A.begin( ) , A.end( ) , 0);
	t.stop( );
	double serialTime = t.getElapsedMs();

	t.start();
	int parallelSum = threadSum( p );
	t.stop();
	double parallelTime = t.getElapsedMs( );

	printSummary( parallelSum , serialSum , parallelTime , serialTime );
	return EXIT_SUCCESS;
}

/************************************************************/

vector<int> 
createVector( size_t N )
{
	vector<int> vec( N );
  	random_device rd;
    mt19937 gen( rd( ) );
    gen.seed( 0 );
  	uniform_int_distribution<int> dist( 0, 4 );

  	generate_n (vec.begin (), N, [&gen, &dist] () { return dist (gen); });

	return vec;
}

/************************************************************/

int 
threadSum( size_t p )
{
	vector<int> localSums( p );
	vector<thread> threads( p );

	size_t sections = ( A.size( ) % p == 0 ) 
					? ( A.size( ) / p ) 
					: ( ( A.size( ) / p ) + 1 );
	size_t offset;
	
	for ( size_t i = 0; i < p; ++i )
	{
		offset = sections * i;
		thread t( localSum , cref( A ) , offset , sections , ref( localSums[ i ]));
		threads[ i ] = move( t );
	}

	for ( thread& t : threads ) t.join();

	return accumulate( localSums.begin(), localSums.end(), 0 );
}

/************************************************************/

void
localSum( const vector<int>& A , size_t offset , size_t sections , int& sum )
{
	if ( A.size( ) > offset + sections )
	{
		sum = accumulate( A.begin( ) + offset, A.begin( ) + offset + sections, 0 );
	}
	else
	{
		sum = accumulate( A.begin( ) + offset, A.end( ), 0 );
	}
}

/************************************************************/

void 
printSummary( int parallelSum , int serialSum , double parallelTime , double serialTime )
{
	cout << "Parallel sum:  " << parallelSum << endl;
	cout << "Parallel time: " << parallelTime << " ms" << endl << endl;

	cout << "Serial sum:    " << serialSum << endl;
	cout << "Serial time:   " << serialTime << " ms" << endl << endl;
}

/************************************************************/
