/************************************************************
	
	FileName	: Row_Reduction.cc
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Final Project
	Description	: Various Serial and Parallel Row Reduction 
				  Algorithms Implementations

************************************************************/
// System includes

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <functional>

#include <boost/thread/barrier.hpp>
#include <thread>

/************************************************************/
// Local includes

#include "Matrix.hpp"
#include "Timer.hpp"

/************************************************************/
// Using declarations

using std::boolalpha;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::ref;
using std::thread;
using boost::barrier;

/************************************************************/
// Function prototypes/global vars/typedefs

void
row_reduce_serial ( Matrix& matrix );

void
row_reduce_thread ( Matrix& matrix, unsigned p );

void
row_reduce_thread_column ( Matrix& matrix, unsigned p );

void
row_reduce_thread_column_opt ( Matrix& matrix, unsigned p, unsigned block_size );

void
row_reduce_helper ( Matrix& matrix, barrier& b, int& non_zero_row, 
	unsigned row_begin, unsigned row_end );

void
row_reduce_helper_column ( Matrix& matrix, barrier& b, 
	vector<double>& first_elements, int& non_zero_row, 
	double& scalar, unsigned column_begin, unsigned column_end );

void
row_reduce_helper_column_opt ( Matrix& matrix, barrier& b, vector<double>& first_elements,
	int& non_zero_row, double& scalar, unsigned column_begin, unsigned column_end, unsigned block_size );

/************************************************************/

int
main ( int argc, char* argv[] )
{
	cout << endl;
	cout << "Print Matrices( y or n ) ";
	char answer;
	cin >> answer;

	cout << "Initialize Matrix Manually( y or n ) ";
	char answer2;
	cin >> answer2;

	unsigned num_rows;
	cout << "Rows       ==> ";
	cin >> num_rows;

	unsigned num_cols;
	cout << "Columns    ==> ";
	cin >> num_cols;

	unsigned p;
	cout << "P          ==> ";
	cin >> p;

	unsigned block_size;
	cout << "Block Size ==> ";
	cin >> block_size;

	Matrix matrix_serial( num_rows , num_cols );
	if ( answer2 == 'y' )
	{
		initalize_test_matrix( matrix_serial );
	}
	else
	{
		generate_matrix( matrix_serial );
	}
	Matrix matrix_thread = matrix_serial;
	Matrix matrix_thread_column = matrix_serial;
	Matrix matrix_thread_column_opt = matrix_serial;
	if ( answer == 'y' ) cout << matrix_serial;

	Timer< > t;
	row_reduce_serial( matrix_serial );
	t.stop();
	double serial_time = t.getElapsedMs();
	if ( answer == 'y' ) 
	{
		cout << "Serial Row Reduce";
		cout << matrix_serial;
	}

	t.start();
	row_reduce_thread( matrix_thread, p );
	t.stop();
	double parallel_time = t.getElapsedMs();
	if ( answer == 'y' ) 
	{
		cout << "Parallel Row Reduce";
		cout << matrix_thread;
	}

	t.start();
	row_reduce_thread_column( matrix_thread_column, p );
	t.stop();
	double parallel_time_column = t.getElapsedMs();
	if ( answer == 'y' ) 
	{
		cout << "Parallel Column Row Reduce";
		cout << matrix_thread_column;
	}

	t.start();
	row_reduce_thread_column_opt( matrix_thread_column_opt, p, block_size );
	t.stop();
	double parallel_time_column_opt = t.getElapsedMs();
	if ( answer == 'y' ) 
	{
		cout << "Parallel Column Row Reduce";
		cout << matrix_thread_column;
	}

	matrix_serial.fix_sign_zeroes();
	matrix_thread.fix_sign_zeroes();
	matrix_thread_column.fix_sign_zeroes();
	matrix_thread_column_opt.fix_sign_zeroes();

	cout << endl << boolalpha;
	cout << "Serial Time:           " << serial_time << endl;
	cout << "Thread Time:           " << parallel_time << endl;
	cout << "Thread Column Time:    " << parallel_time_column << endl;
	cout << "Thread Block Time:     " << parallel_time_column_opt << endl;
	cout << "Matrices equal:        " 
		 << (( matrix_serial == matrix_thread )
		  && ( matrix_serial == matrix_thread_column )
		  && ( matrix_serial == matrix_thread_column_opt ));
	cout << endl << endl;

	return EXIT_SUCCESS;
}

/************************************************************/

void
row_reduce_serial ( Matrix& matrix )
{
	unsigned num_diagonal_elements = 0;
	int non_zero_row;
	for ( unsigned j = 0; ( j < matrix.get_columns() ) 
		&& ( num_diagonal_elements < matrix.get_rows() ); ++j )
	{
		non_zero_row = matrix.find_nonzero( num_diagonal_elements , j );

		if ( non_zero_row >= 0 )
		{
			for ( int i = 0; i < matrix.get_rows(); ++i )
			{
				if ( i != non_zero_row && matrix[ i ][ j ] != 0 )
				{
					matrix.row_operation ( i, non_zero_row, matrix[ i ][ j ] );
				}
			}
			++num_diagonal_elements;
		}
	}
}

/************************************************************/

void
row_reduce_thread ( Matrix& matrix, unsigned p )
{
	vector<thread> threads( p );
	barrier b( p );
	unsigned row_sections = matrix.get_rows() / p;
	unsigned row_begin = 0;
	unsigned row_end = 0;
	int non_zero_row = 0;

	for ( unsigned i = 0; i < p; ++i )
	{
		row_begin = row_end;
		if ( i == p - 1 )
		{
			row_end = matrix.get_rows();
		}
		else
		{
			row_end += row_sections;
		}
		threads[ i ] = thread( row_reduce_helper, ref( matrix ), ref( b ),
			ref( non_zero_row ), row_begin, row_end );
	}

	for ( unsigned i = 0; i < p; ++i ) threads[ i ].join();
}

/************************************************************/

void
row_reduce_thread_column ( Matrix& matrix, unsigned p )
{
	vector<thread> threads( p );
	vector<double> first_elements( matrix.get_rows() );
	barrier b( p );
	unsigned column_sections = matrix.get_columns() / p;
	unsigned column_begin = 0;
	unsigned column_end = 0;
	int non_zero_row = 0;
	double scalar = 0;

	for ( unsigned i = 0; i < p; ++i )
	{
		column_begin = column_end;
		if ( i == p - 1 )
		{
			column_end = matrix.get_columns();
		}
		else
		{
			column_end += column_sections;
		}
		threads[ i ] = thread( row_reduce_helper_column, ref( matrix ),
			ref( b ), ref( first_elements ), ref( non_zero_row ),  
			ref( scalar ), column_begin, column_end );
	}

	for ( unsigned i = 0; i < p; ++i ) threads[ i ].join();
}

/************************************************************/
// Made for large sets of data

void
row_reduce_thread_column_opt ( Matrix& matrix, unsigned p, unsigned block_size )
{
	vector<thread> threads( p );
	vector<double> first_elements( matrix.get_rows() );
	barrier b( p );
	unsigned column_sections = matrix.get_columns() / p;
	unsigned column_begin = 0;
	unsigned column_end = 0;
	int non_zero_row = 0;
	double scalar = 0;

	for ( unsigned i = 0; i < p; ++i )
	{
		column_begin = column_end;
		if ( i == p - 1 )
		{
			column_end = matrix.get_columns();
		}
		else
		{
			column_end += column_sections;
		}
		threads[ i ] = thread( row_reduce_helper_column_opt, ref( matrix ),
			ref( b ), ref( first_elements ), ref( non_zero_row ),  
			ref( scalar ), column_begin, column_end, block_size );
	}

	for ( unsigned i = 0; i < p; ++i ) threads[ i ].join();
}

/************************************************************/
// [ row_begin , row_end )

void
row_reduce_helper ( Matrix& matrix, barrier& b, int& non_zero_row, 
	unsigned row_begin, unsigned row_end )
{
	unsigned num_diagonal_elements = 0;
	for ( unsigned j = 0; ( j < matrix.get_columns() ) 
		&& ( num_diagonal_elements < matrix.get_rows() ); ++j )
	{		
		if ( row_begin == 0 )
		{
			non_zero_row = matrix.find_nonzero( num_diagonal_elements , j );
		}
		b.wait();

		if ( non_zero_row >= 0 )
		{
			for ( int i = row_begin; i < row_end; ++i )
			{
				if ( i != non_zero_row && matrix[ i ][ j ] != 0 )
				{
					matrix.row_operation ( i, non_zero_row, matrix[ i ][ j ] );
				}
			}
			++num_diagonal_elements;
			b.wait();
		}
	}
}

/************************************************************/

void
row_reduce_helper_column ( Matrix& matrix, barrier& b, vector<double>& first_elements,
	int& non_zero_row, double& scalar, unsigned column_begin, unsigned column_end )
{
	int num_rows = matrix.get_rows();
	int num_diagonal_elements = 0;
	for ( unsigned j = 0; ( j < matrix.get_columns() ) 
		&& ( num_diagonal_elements < matrix.get_rows() ); ++j )
	{
		if ( column_begin == 0 )
		{	
			non_zero_row = matrix.find_nonzero_column( num_diagonal_elements , j );
			scalar = matrix[ non_zero_row ][ j ];

			if ( non_zero_row != -1 )
			{
				if ( num_diagonal_elements == non_zero_row )
				{
					for ( unsigned i = 0; i < matrix.get_rows(); ++i )
					{
						first_elements[ i ] = matrix[ i ][ j ];
					}
				}
				else
				{
					for ( int i = 0; i < num_rows; ++i )
					{
						if ( i != non_zero_row && i != num_diagonal_elements )
						{
							first_elements[ i ] = matrix[ i ][ j ];
						}
						else if ( i == num_diagonal_elements )
						{
							first_elements[ i ] = 1;
						}
						else
						{
							first_elements[ i ] = matrix[ num_diagonal_elements ][ j ];
						}
					}
				}
			}
		}
		b.wait();

		if ( non_zero_row >= 0 )
		{
			if ( num_diagonal_elements != non_zero_row )
			{
				matrix.swap_rows_column ( num_diagonal_elements, non_zero_row, 
					column_begin, column_end );
			}

			if ( scalar != 1 )
			{
				matrix.row_divisor_column ( num_diagonal_elements, column_begin, 
					column_end, scalar );
			}

			for ( int i = 0; i < num_rows; ++i )
			{
				if ( i != num_diagonal_elements && first_elements[ i ] != 0 )
				{
					matrix.row_operation_column ( i, non_zero_row, column_begin, 
						column_end, first_elements[ i ] );
				}
			}
			++num_diagonal_elements;
			b.wait();
		}
	}
}

/************************************************************/

void
row_reduce_helper_column_opt ( Matrix& matrix, barrier& b, vector<double>& first_elements,
	int& non_zero_row, double& scalar, unsigned column_begin, unsigned column_end, unsigned block_size )
{
	int num_rows = matrix.get_rows();
	int num_diagonal_elements = 0;
	for ( unsigned j = 0; ( j < matrix.get_columns() ) 
		&& ( num_diagonal_elements < matrix.get_rows() ); ++j )
	{
		if ( column_begin == 0 )
		{	
			non_zero_row = matrix.find_nonzero_column( num_diagonal_elements , j );
			scalar = matrix[ non_zero_row ][ j ];

			if ( non_zero_row != -1 )
			{
				if ( num_diagonal_elements == non_zero_row )
				{
					for ( unsigned i = 0; i < matrix.get_rows(); ++i )
					{
						first_elements[ i ] = matrix[ i ][ j ];
					}
				}
				else
				{
					for ( int i = 0; i < num_rows; ++i )
					{
						if ( i != non_zero_row && i != num_diagonal_elements )
						{
							first_elements[ i ] = matrix[ i ][ j ];
						}
						else if ( i == num_diagonal_elements )
						{
							first_elements[ i ] = 1;
						}
						else
						{
							first_elements[ i ] = matrix[ num_diagonal_elements ][ j ];
						}
					}
				}
			}
		}
		b.wait();

		if ( non_zero_row >= 0 )
		{
			unsigned size = column_end - column_begin;
			if ( size < 2 * block_size )
			{
				if ( num_diagonal_elements != non_zero_row )
				{
					matrix.swap_rows_column ( num_diagonal_elements, non_zero_row, 
					column_begin, column_end );
				}

				if ( scalar != 1 )
				{
					matrix.row_divisor_column ( num_diagonal_elements, column_begin, 
					column_end, scalar );
				}

				for ( int i = 0; i < num_rows; ++i )
				{
					if ( i != num_diagonal_elements && first_elements[ i ] != 0 )
					{
						matrix.row_operation_column ( i, non_zero_row, column_begin, 
						column_end, first_elements[ i ] );
					}
				}
			}
			else
			{
				unsigned iterations = size / block_size;
				unsigned curr_begin;
				unsigned curr_end = column_begin;

				if ( num_diagonal_elements != non_zero_row )
				{
					for ( unsigned i = 0; i < iterations; ++i )
					{
						curr_begin = curr_end;
						if ( i == ( iterations - 1 ))
						{
							curr_end = column_end;
						}
						else
						{
							curr_end += block_size;
						}
						matrix.swap_rows_column ( num_diagonal_elements, non_zero_row, 
							curr_begin, curr_end );
					}
				}
				curr_end = column_begin;

				if ( scalar != 1 )
				{
					for ( unsigned i = 0; i < iterations; ++i )
					{
						curr_begin = curr_end;
						if ( i == ( iterations - 1 ))
						{
							curr_end = column_end;
						}
						else
						{
							curr_end += block_size;
						}
						matrix.row_divisor_column ( num_diagonal_elements, curr_begin, 
							curr_end, scalar );
					}
				}
				curr_end = column_begin;

				for ( unsigned i = 0; i < iterations; ++i )
				{
					curr_begin = curr_end;
					if ( i == ( iterations - 1 ))
					{
						curr_end = column_end;
					}
					else
					{
						curr_end += block_size;
					}

					for ( int i = 0; i < num_rows; ++i )
					{
						if ( i != num_diagonal_elements && first_elements[ i ] != 0 )
						{
							matrix.row_operation_column ( i, non_zero_row, curr_begin, 
								curr_end, first_elements[ i ] );
						}
					}
				}
			}
			
			++num_diagonal_elements;
			b.wait();
		}
	}
}

/************************************************************/