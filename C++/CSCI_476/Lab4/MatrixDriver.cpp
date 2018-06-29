/*
	FileName	: MatrixDriver.cpp
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Lab 4
	Description	: This program shows the different versions of
					matrix multiplication and the times.
*/

/************************************************************/
// System includes

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <random>

/************************************************************/
// Local includes

#include "Timer.hpp"
#include "Matrix.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::uniform_int_distribution;
using std::mt19937;

/************************************************************/
// Function prototypes/global vars/typedefs

Matrix<int>
createMatrix( unsigned numRows , unsigned numCols );

int
blockSum( Matrix<int>& A , Matrix<int>& B , unsigned blockSize );

int
ijkSum( Matrix<int>& A , Matrix<int>& B );

int
jkiSum( Matrix<int>& A , Matrix<int>& B );

int
kijSum( Matrix<int>& A , Matrix<int>& B );

void
printSummary( int sum, double time );

#define CORRECT 0
mt19937 gen( 0 );
uniform_int_distribution<int> dist( 0, 4 );

/************************************************************/

int
main(int argc, char* argv[])
{
	unsigned N;
	unsigned blockSize = 0;
	int sum = 0;
	double time = 0;
	Timer< > t;
	string version;

	cout << endl << "N       ==> ";
	cin >> N;
	cout << "Version ==> ";
	cin >> version;

	Matrix<int> A = createMatrix ( N , N );
	Matrix<int> B = createMatrix ( N , N );

	if ( version.compare( "block" ) == CORRECT )
	{
		cout << "BS      ==> ";
		cin >> blockSize;
	}
	cout << endl;

	if ( version.compare( "block" ) == CORRECT )
	{
		t.start();
		sum = blockSum( A , B , blockSize );
		t.stop( );
	}
	else if ( version.compare( "ijk" ) == CORRECT )
	{
		t.start();
		sum = ijkSum( A , B );
		t.stop( );
	}
	else if ( version.compare( "jki" ) == CORRECT )
	{
		t.start();
		sum = jkiSum( A , B );
		t.stop( );
	}
	else if ( version.compare( "kij" ) == CORRECT )
	{
		t.start();
		sum = kijSum( A , B );
		t.stop( );
	}
	time = t.getElapsedMs();
	printSummary( sum , time );

	return EXIT_SUCCESS;
}

/************************************************************/

Matrix<int>
createMatrix( unsigned numRows , unsigned numCols )
{
  	Matrix<int> m( numRows , numCols );
  	
  	for ( unsigned i = 0; i < numRows; ++i )
  	{
  		for ( unsigned j = 0; j < numCols; ++j )
  		{
  			m( i , j ) = dist( gen );
  		}
  	}
  	return m;
}

/************************************************************/

int
blockSum( Matrix<int>& A , Matrix<int>& B , unsigned blockSize )
{
	int sum = 0;
	for ( int i = 0; i < A.getRows(); i += blockSize )
	{
		for ( int j = 0; j < B.getColumns(); j += blockSize )
		{
			for ( int k = 0; k < A.getColumns(); k += blockSize )
			{
				for ( int i1 = i; i1 < i + blockSize; ++i1 )
				{
					for ( int j1 = j; j1 < j + blockSize; ++j1 )
					{
						for ( int k1 = k; k1 < k + blockSize; ++k1 )
						{
							sum += A( i1 , k1 ) * B( k1 , j1 );
						}
					}
				}
			}
		}
	}
	return sum;
}

/************************************************************/

int
ijkSum( Matrix<int>& A , Matrix<int>& B )
{
	int sum = 0;
	for ( int i = 0; i < A.getRows(); ++i )
	{
		for ( int j = 0; j < B.getColumns(); ++j )
		{
			for ( int k = 0; k < A.getColumns(); ++k )
			{
				sum += A( i , k ) * B( k , j );
			}
		}
	}
	return sum;
}

/************************************************************/

int
jkiSum( Matrix<int>& A , Matrix<int>& B )
{
	int sum = 0;
	int jkElement;
	for ( int j = 0; j < B.getColumns(); ++j )
	{
		for ( int k = 0; k < A.getColumns(); ++k )
		{
			jkElement = B( k , j );
			for ( int i = 0; i < A.getRows(); ++i )
			{
				sum += A ( i , k ) * jkElement;
			}
		}
	}
	return sum;
}

/************************************************************/

int
kijSum( Matrix<int>& A , Matrix<int>& B )
{
	int sum = 0;
	int ikElement = 0;
	for ( int k = 0; k < A.getColumns(); ++k )
	{
		for ( int i = 0; i < A.getRows(); ++i )
		{
			ikElement = A( i , k );
			for ( int j = 0; j < B.getColumns(); ++j )
			{
				sum += ikElement * B( k , j );
			}
		}
	}
	return sum;
}

/************************************************************/

void
printSummary( int sum, double time )
{
	cout << "Sum:  " << sum << endl;
	cout << "Time: " << time << " ms" << endl;
}

/*************************************************************
N      ijk         jki        kij        block
====   =========   ========   ========   ========
1024   10215.924   6777.434   5849.652	 7791.308
1408   26499.48    18236.23   15186.78   19253.94
1792   65998.32    45980.65   31698.46   40313.19

Discussion
==========
Overall, the KIJ algorithm is the fastest like we discussed in
class, JKI was a close second, and block is a close third with
IJK as the slowest amongst the algorithms. The ranking of speeds
fit the description that we discussed in class. However, the
speeds were not proportional to the cache misses that greatly
affect the time of these algorithms.
*************************************************************/