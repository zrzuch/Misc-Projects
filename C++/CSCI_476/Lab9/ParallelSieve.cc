/************************************************************
	FileName	: ParallelSieve.cc
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Lab 9
	Description	: Parallel Sieve of Eratosthenes
				Check Earlier submit for solution that is of 
				the correct format

************************************************************/
// System includes

#include <cstdlib>
#include <iostream>
#include <vector>
#include <thread>
#include <functional>

#include <omp.h>
#include <cilk/cilk.h>

/************************************************************/
// Local includes

#include "Timer.hpp"

/************************************************************/
// Using declarations

using std::boolalpha;
using std::cin;
using std::cout;
using std::endl;
using std::ref;
using std::thread;
using std::vector;

/************************************************************/
// Function prototypes/global vars/typedefs

using uint = unsigned;

bool
is_equal( uint a, uint b, uint c );

uint
serial_sieve( uint n );

uint
sieve ( uint N, uint p );

uint
sieve_cilk ( uint n, uint p );

uint
sieve_omp ( uint n, uint p );

uint
calculate_sections( uint n, uint p );

void
block_sieve_vector( uint begin, uint end, uint& out );

void
block_sieve( uint begin, uint end, uint& out );

uint
block_sieve_return( uint begin, uint end );

bool
is_divisible( uint n );

vector< uint > small_primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 
31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 
103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 
179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 
257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 
347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 
431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 
509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593,	599, 601,
607, 613, 617, 619,	631, 641, 643, 647,	653, 659, 661, 673,	677, 683,
691, 701, 709, 719,	727, 733, 739, 743,	751, 757, 761, 769,	773, 787,	
797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
883, 887, 907, 911,	919, 929, 937, 941, 947, 953, 967, 971,	977, 983,
991, 997, 1009, 1013, 1019,	1021, 1031,	1033, 1039, 1049, 1051, 1061,
1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151,
1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223 };

/************************************************************/

// 0 = Cilk
// 1 = OpenMP
// 2 = C++ Threads

int
main ( int argc, char* argv[] )
{
	uint N;
	uint p;
	uint num_primes [ 3 ];
	double times [ 3 ];

	cout << endl << "N ==> ";
	cin >> N;
	cout << "p ==> ";
	cin >> p;
	cout << endl;

	Timer< > t;
	num_primes[ 0 ] = sieve_cilk( N, p );
	t.stop( );
	times[ 0 ] = t.getElapsedMs();

	t.start();
	num_primes[ 1 ] = sieve_omp( N, p );
	t.stop( );
	times[ 1 ] = t.getElapsedMs();

	t.start();
	num_primes[ 2 ] = sieve( N, p );
	t.stop( );
	times[ 2 ] = t.getElapsedMs();

	cout << "Pi[" << N << "] = " << num_primes[ 0 ] << endl;
	cout << "Cilk Time: " << times[ 0 ] << endl;
	cout << "OpenMP Time: " << times[ 1 ] << endl;
	cout << "Thread Time: " << times[ 2 ] << endl;
	cout << boolalpha << "Check Cilk, OpenMP, Thread: " 
		<< is_equal( num_primes[0], num_primes[1], num_primes[2]) 
		<< endl << endl;

	return EXIT_SUCCESS;
}

/************************************************************/

bool
is_equal( uint a, uint b, uint c )
{
	if ( a == b )
	{
		if ( a == c )
		{
			if ( b == c ) return true;
		}
		else return false;
	}
	else return false;
}

/************************************************************/

uint
serial_sieve( uint n )
{
	vector<bool> primes (( n - 1 ) / 2, true );
	uint num_primes = 1;

	for ( uint curr_num = 3; curr_num * curr_num <= n; curr_num += 2 )
	{
		if ( primes[ ( curr_num - 3 ) / 2 ] )
		{
			for ( uint j = curr_num * curr_num; j <= n; j += 2 * curr_num )
			{
				primes[ ( j - 3 ) / 2 ] = false;
			}
		}
	}

	for ( uint i = 0; i < primes.size(); ++i )
	{
		if ( primes[i] ) { ++num_primes; }
	}

	return num_primes;
}

/************************************************************/

uint
sieve_omp ( uint n, uint p )
{
	uint sections = calculate_sections( n, p );
	omp_set_num_threads( p );
	uint num_primes = 0;

	#pragma omp parallel for reduction( +:num_primes )
	for ( uint begin = 2; begin <= n; begin += sections )
	{
		uint end = begin + sections;
		if ( end > n )
		{
			end = n;
		}

		num_primes += block_sieve_return( begin, end );
	}

	return num_primes;
}

/************************************************************/

uint
sieve_cilk ( uint n, uint p )
{
	uint sections = calculate_sections( n, p );
	uint num_primes = 0;


	cilk_for ( uint begin = 2; begin <= n; begin += sections )
	{
		uint end = begin + sections;
		if ( end > n )
		{
			end = n;
		}

		num_primes += block_sieve_return( begin, end );
	}

	return num_primes;
}

/************************************************************/

uint
sieve ( uint n, uint p )
{
	vector< thread > threads( p );
	vector< uint > local_sums( p );
	uint sections = ( n - 1 ) / p;
	uint begin = 0;
	uint end = 1;
	uint sum = 0;

	for ( uint i = 0; i < p; ++i )
	{
		begin = end + 1;
		end = begin + sections;
		threads[ i ] = thread( block_sieve, begin, end, 
			ref( local_sums[ i ]));
	}

	for ( uint i = 0; i < p; ++i ) 
	{
		threads[ i ].join();
	}

	for ( auto var : local_sums ) 
	{
		sum += var;
	}

	return sum;
}

/************************************************************/

uint
calculate_sections( uint n, uint p )
{
	if ( n == 1000000000 )
	{
		return n / 310;
	}
	else if ( n > 1000000 )
	{
		return ( n - 1 ) / ( p * p );
	}
	else
	{
		return n;
	}
}

/************************************************************/

void
block_sieve( uint begin, uint end, uint& out )
{
	uint mem_size = ( end - begin + 1 ) / 2;
	unsigned char* primes = new unsigned char[ mem_size ];
	for ( uint i = 0; i < mem_size; ++i )
	{
		primes[ i ] = 1;
	}

	for ( uint i = 3; i * i <= end; i += 2 )
	{
		if ( is_divisible( i ) ) 
		{
			continue;
		}

		uint min = (( begin + ( i - 1 )) / i ) * i;
		if ( min < i * i ) 
		{
			min = i * i;
		}
		if ( min % 2 == 0 ) 
		{
			min += i; 
		}

		for ( uint j = min; j <= end; j += 2 * i )
		{
			uint index = j - begin;
			primes[ index / 2 ] = 0;
		}
	}

	uint num_primes = ( begin <= 2 ) ? 1 : 0;
	for ( uint i = 0; i < mem_size; ++i )
	{
		num_primes += primes[ i ];
	}

	delete[] primes;
	out = num_primes;
}

/************************************************************/

uint
block_sieve_return( uint begin, uint end )
{
	uint mem_size = ( end - begin + 1 ) / 2;
	unsigned char* primes = new unsigned char[ mem_size ];
	#pragma simd
	for ( uint i = 0; i < mem_size; ++i )
	{
		primes[ i ] = 1;
	}

	for ( uint i = 3; i * i <= end; i += 2 )
	{
		if ( is_divisible( i ) ) 
		{
			continue;
		}

		uint min = (( begin + ( i - 1 )) / i ) * i;
		if ( min < i * i ) 
		{
			min = i * i;
		}
		if ( min % 2 == 0 ) 
		{
			min += i; 
		}

		for ( uint j = min; j <= end; j += 2 * i )
		{
			uint index = j - begin;
			primes[ index / 2 ] = 0;
		}
	}

	uint num_primes = ( begin <= 2 ) ? 1 : 0;
	#pragma simd
	for ( uint i = 0; i < mem_size; ++i )
	{
		num_primes += primes[ i ];
	}

	delete[] primes;
	return num_primes;
}

/************************************************************/

bool
is_divisible( uint n )
{
	for ( uint i = 0; i < small_primes.size(); ++i )
	{
		if ( n > small_primes[ i ] )
		{
			if ( n % small_primes[ i ] == 0 ) 
			{
				return true;
			}
		}
		else 
		{
			break;
		}
	}
	return false;
}

/************************************************************/