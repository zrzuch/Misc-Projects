/*
	FileName	: Forking.cc
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Lab 2
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
#include <functional>
#include <cassert>

#include <unistd.h>
#include <sys/wait.h>

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

/************************************************************/
// Function prototypes/global vars/typedefs

vector<int> 
createVector( size_t N );

int 
forkSum( size_t p );

void 
printSummary( int parallelSum , int serialSum , double parallelTime , double serialTime );

vector<int> g_A;
#define READ  0
#define WRITE 1

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
	
	g_A = createVector( N );

	Timer< > t;
	int serialSum = accumulate( g_A.begin( ) , g_A.end( ) , 0);
	t.stop( );
	double serialTime = t.getElapsedMs();

	t.start();
	int parallelSum = forkSum( p );
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
    
  	for ( auto &num : vec )
  	{
  		num = dist( gen );
  	}

	return vec;
}

/************************************************************/

int 
forkSum( size_t p )
{
	int localSum = 0;
	size_t sections = ( g_A.size( ) % p == 0 ) 
					? ( g_A.size( ) / p ) 
					: ( ( g_A.size( ) / p ) + 1 );
	size_t offset;

	int fd[ 2 ];
	pid_t childPid;
	pipe( fd );
	
	for ( size_t i = 0; i < p; ++i )
	{
		offset = sections * i;
		childPid = fork( );
		assert ( childPid != -1 );

		if ( childPid == 0 )
		{
			if ( i > 0 )
			{
				read( fd[ READ ] , &localSum , sizeof( localSum ) );
			}

			if ( g_A.size( ) > offset + sections )
			{
				localSum = accumulate( g_A.begin( ) + offset, g_A.begin( ) + offset + sections , localSum );
			}
			else
			{
				localSum = accumulate( g_A.begin( ) + offset, g_A.end( ) , localSum );
			}

			close( fd[ READ ] );
			write( fd[ WRITE ] , &localSum , sizeof( localSum ) );
			close( fd[ WRITE ] );
			exit( EXIT_SUCCESS );
		}
	}

	for ( size_t i = 0; i < p; ++i )
	{
		wait( 0 );
	}

	read( fd[ READ ] , &localSum , sizeof( localSum ) );
	return localSum;
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
