/*
	FileName	: ParallelGameOfLife.cc
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Lab 8
	Description	: Parallel Game of Life Simulation
*/

/************************************************************/
// System includes

#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>

#include <boost/thread/barrier.hpp>

/************************************************************/
// Local includes

#include "Timer.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::minstd_rand;
using std::copy;
using std::ios;
using std::ifstream;
using std::ofstream;
using std::count;
using std::istream_iterator;
using std::thread;
using std::ref;
using std::ostream;

using boost::barrier;

/************************************************************/
// Function prototypes/global vars/typedefs

vector<vector<bool>>
generate_bool_matrix ( unsigned N, unsigned seed );

vector<vector<bool>>
generate_bool_matrix ( string filePath );

void
write_matrix_to_file ( vector< vector<bool> >& life, ostream& outStream );

void
game_of_life ( vector< vector<bool> >& life, unsigned p, unsigned generations );

void
generations_helper ( vector< vector<bool> >& life, vector< vector<bool> >& life_buffer,
	unsigned begin_pos, unsigned end_pos, unsigned generations, barrier& b );

bool
check_if_alive ( vector< vector<bool> >& life, unsigned i, unsigned j );

/************************************************************/

int
main ( int argc, char* argv[] )
{
	vector<vector<bool>> life;
	string file;
	unsigned p;
	unsigned gens;

	cout << endl << "Input file  ==> ";
	cin >> file;

	if ( file == "n" )
	{
		unsigned N;
		unsigned seed;

		cout << "N           ==> ";
		cin >> N;
		cout << "Seed        ==> ";
		cin >> p;

		life = generate_bool_matrix( N, seed );
	}
	else
	{
		life = generate_bool_matrix ( file );
	}

	cout << "p           ==> ";
	cin >> p;
	cout << "Generations ==> ";
	cin >> gens;
	cout << endl;

	Timer<> t;
	game_of_life( life, p, gens );
	t.stop();
	cout << "Parallel time:  " << t.getElapsedMs() << " ms" << endl;

	ofstream bitFile( "GameOfLifeOutput.bin", ios::out | ios::binary );
	write_matrix_to_file( life, bitFile );
	bitFile.close ();
	cout << "Output file  :  GameOfLifeOutput.bin" << endl  << endl;
	
	return EXIT_SUCCESS;
}

/************************************************************/

vector<vector<bool>>
generate_bool_matrix ( unsigned N, unsigned seed )
{
	vector<vector<bool>> bool_matrix(( N + 2 ), vector<bool>( N + 2 ));
	minstd_rand gen( seed );
	std::bernoulli_distribution dist;
	
	for ( unsigned i = 1; i <= N; ++i )
	{
		for ( unsigned j = 1; j <= N; ++j )
		{
			bool_matrix[ i ][ j ] = dist( gen );
		}
	}
	return bool_matrix;
}

/************************************************************/

vector<vector<bool>>
generate_bool_matrix ( string filePath )
{
	ifstream file( filePath );
	file.unsetf(std::ios_base::skipws);
	unsigned N = count( istream_iterator<char>( file ), 
		istream_iterator<char>(), '\n' );
	vector<vector<bool>> bool_matrix(( N + 2 ), vector<bool>( N + 2 ));
	char cell;
	
	for ( unsigned i = 1; i <= N; ++i )
	{
		for ( unsigned j = 1; j <= N; ++j )
		{
			file >> cell;
			if ( cell == '#' )
			{
				bool_matrix[ i ][ j ] = true;
			}
		}
	}
	return bool_matrix;
}

/************************************************************/

void
write_matrix_to_file ( vector< vector<bool> >& life, ostream& outStream )
{
	vector<unsigned char> bytes(( life.size() + 7 ) / 8 );
	char bitCount = 0;
	unsigned byteIndex = 0;

	for ( unsigned line = 0; line < life.size(); ++line )
	{
		for ( auto bit : life[ line ] )
		{
			bytes[ byteIndex ] = ( bytes[ byteIndex ] << 1 ) | bit;
			++bitCount;
			if ( bitCount == 8 )
			{
				++byteIndex;
				bitCount = 0;
			}
		}

		if ( bitCount != 0 )
		{
			bytes[ byteIndex ] << ( 8 - bitCount );
		}

		outStream.write( reinterpret_cast<char*>( bytes.data() ), bytes.size() );
	}
}

void
game_of_life ( vector< vector<bool> >& life, unsigned p, unsigned generations )
{
	vector< vector<bool> > life_buffer( life.size() - 2 , 
			vector<bool>( life.size() -2 ));
	vector<thread> threads( p );
	barrier b( p );

	unsigned num_cells = ( life.size() - 2 ) * ( life.size() - 2 );
	unsigned section_size = num_cells / p;
	int extra_cells = num_cells % p;
	unsigned prev_end = 0;
	unsigned begin_pos;
	unsigned end_pos;

	for ( unsigned i = 0; i < p; ++i )
	{
		begin_pos = prev_end;
		end_pos = begin_pos + section_size;

		if ( extra_cells != 0 )
		{
			++end_pos;
			--extra_cells;
		}
		prev_end = end_pos;

		threads[ i ] = thread( generations_helper, ref( life ), ref( life_buffer ), 
			begin_pos, end_pos, generations, ref( b ));
	}

	for ( unsigned i = 0; i < p; ++i ) threads[ i ].join();
}

/************************************************************/

void
generations_helper ( vector< vector<bool> >& life, vector< vector<bool> >& life_buffer,
	unsigned begin_pos, unsigned end_pos, unsigned generations, barrier& b )
{
	unsigned n = life.size() - 2;
	unsigned begin_i = ( begin_pos / n ) + 1;
	unsigned begin_j = ( begin_pos % n ) + 1;
	unsigned end_i = ( end_pos / n ) + 1;
	unsigned end_j = ( end_pos % n ) + 1;
	
	unsigned start_value;
	unsigned end_value;
	unsigned i;
	unsigned j;

	for ( unsigned curr_gen = 0; curr_gen < generations; ++curr_gen )
	{
		for ( i = begin_i; i <= end_i; ++i )
		{
			start_value = ( i == begin_i ? begin_j : 1 );
			end_value = ( i == end_i ? end_j : n );
			for ( j = start_value; j < end_value; ++j )
			{
				life_buffer[ i - 1 ][ j - 1 ] = check_if_alive( life, i, j );
			}
		}
		b.wait();

		for ( i = begin_i; i <= end_i; ++i )
		{
			start_value = ( i == begin_i ? begin_j : 1 );
			end_value = ( i == end_i ? end_j : n ); 
			for ( j = start_value; j < end_value; ++j )
			{
				life[ i ][ j ] = life_buffer[ i - 1 ][ j - 1 ];
			}
		}
		b.wait();
	}
}

/************************************************************/

bool
check_if_alive ( vector< vector<bool> >& life, unsigned i, unsigned j )
{
	unsigned num_alive_neighbours = 0;

	for ( int i_mod = -1; i_mod <= 1; ++i_mod )
	{
		for ( int j_mod = -1; j_mod <= 1; ++j_mod )
		{
			if ( i_mod == 0 && j_mod == 0 )
			{
				++j_mod;
			}
			if ( life[ i + i_mod ][ j + j_mod ] )
			{
				++num_alive_neighbours;
			}
		}
	}

	if ( num_alive_neighbours < 2 || num_alive_neighbours > 3 )
	{
		return false;
	}
	else if ( life[ i ][ j ] && num_alive_neighbours == 2 )
	{
		return true;
	}
	else
	{
		return true;
	}
}

/************************************************************/