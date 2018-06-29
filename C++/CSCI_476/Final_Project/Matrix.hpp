/************************************************************
	
	FileName	: Matrix.hpp
	Author		: Zachary Zuch
	Course		: CSCI 476
	Assignment 	: Final Project
	Description	: A class that creates a real matrix to row reduce
					with real row operations defined in the class

************************************************************/

#ifndef MATRIX_HPP
#define MATRIX_HPP

/************************************************************/
// System includes

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

/************************************************************/
// Function prototypes/global vars/typedefs

using std::cin;
using std::cout;
using std::endl;
using std::mt19937;
using std::ostream;
using std::random_device;
using std::signbit;
using std::uniform_int_distribution;
using std::vector;

/************************************************************/

class Matrix
{
public:

	Matrix ( unsigned rows , unsigned cols )
	: 	m_data( rows , vector<double>( cols ))
	{
		assert ( rows != 0 && cols != 0 );
		// All of these operations can be done on any size matrix,
		// but in implementation, the rows are less than or equal
		// to the columns. 
		assert ( rows <= cols );
	}

	vector<double>&
	operator[] ( unsigned index )
	{
		return m_data[ index ];
	}

	const vector<double>&
	operator[] ( unsigned index ) const
	{
		return m_data[ index ];
	}

	Matrix&
  	operator= ( const Matrix& m )
  	{
    	m_data = m.m_data;
    	return ( *this );
  	}

  	bool
  	operator== ( const Matrix& m )
  	{
  		if ( get_rows() != m.get_rows() ) return false;
  		if ( get_columns() != m.get_columns() ) return false;

  		for ( unsigned i = 0; i < get_rows(); ++i )
  		{
  			for ( unsigned j = 0; j < get_columns(); ++j )
  			{
  				if ( m_data[ i ][ j ] != m[ i ][ j ] ) return false;
  			}
  		}
  		return true;
  	}

	unsigned
  	get_rows( )
  	{
    	return m_data.size( );
  	}

  	unsigned
  	get_rows( ) const
  	{
    	return m_data.size( );
  	}

  	unsigned
  	get_columns( )
  	{
    	return m_data[ 0 ].size( );
 	}

 	unsigned
  	get_columns( ) const
  	{
    	return m_data[ 0 ].size( );
 	}

 	void
 	fix_sign_zeroes ( )
 	{
 		for ( unsigned i = 0; i < get_rows(); ++i )
 		{
 			for ( unsigned j = 0; j < get_columns(); ++j )
 			{
 				if ( signbit ( m_data[ i ][ j ] ) && m_data[ i ][ j ] == 0 )
				{
					m_data[ i ][ j ] = 0;
				}
 			}
 		}
 	}

	void
	swap_rows ( unsigned row1 , unsigned row2 )
	{
		assert ( row1 <= get_rows() && row2 <= get_rows() );

		vector<double> temp = m_data[ row1 ];
		m_data[ row1 ] = m_data[ row2 ];
		m_data[ row2 ] = temp;
	}

	void
	swap_rows_column ( unsigned row1 , unsigned row2, 
		unsigned column_begin, unsigned column_end )
	{
		assert ( row1 <= get_rows() && row2 <= get_rows() );
		unsigned size = column_end - column_begin;

		vector<double> temp( size );
		for ( unsigned i = 0; i < size; ++i )
		{
			temp[ i ] = m_data[ row1 ][ column_begin + i ];
		}

		for ( unsigned j = column_begin; j < column_end; ++j )
		{
			m_data[ row1 ][ j ] = m_data[ row2 ][ j ];
		}

		for ( unsigned i = 0; i < size; ++i )
		{
			m_data[ row2 ][ column_begin + i ] = temp[ i ];
		}
	}

	// Param: 
	//	row_applied - row that will not be changed
	//	row_changed - row that will be subtracted by the row_applied
	//					by a scalar multiple
	// 	scalar		- the multiple of the row_applied that will be 
	//					subtracted from row_changed
	void
	row_operation ( unsigned row_changed, unsigned row_applied, double scalar )
	{
		assert ( row_changed <= get_rows() && row_applied <= get_rows());
		assert ( scalar != 0 );

		if ( scalar != 1 )
		{
			for ( unsigned i = 0; i < get_columns(); ++i )
			{
				m_data[ row_changed ][ i ] -= ( scalar * m_data[ row_applied ][ i ]);
			}
		}
		else
		{
			for ( unsigned i = 0; i < get_columns(); ++i )
			{
				m_data[ row_changed ][ i ] -= m_data[ row_applied ][ i ];
			}
		}
	}

	void
	row_operation_column ( unsigned row_changed, unsigned row_applied, 
		unsigned column_begin, unsigned column_end, double scalar )
	{
		assert ( row_changed <= get_rows() && row_applied <= get_rows());
		assert ( scalar != 0 );

		if ( scalar != 1 )
		{
			for ( unsigned j = column_begin; j < column_end; ++j )
			{
				m_data[ row_changed ][ j ] -= ( scalar * m_data[ row_applied ][ j ]);
			}
		}
		else
		{
			for ( unsigned j = column_begin; j < column_end; ++j )
			{
				m_data[ row_changed ][ j ] -= m_data[ row_applied ][ j ];
			}
		}
	}

	// Technically, you can only multiply by the inverse of a number, 
	// but since this is a real matrix that means the inverse for any
	// number a is 1/a, so in order to row reduce, we 
	void
	row_divisor ( unsigned row, double scalar )
	{
		assert( scalar != 0 );
		for ( unsigned j = 0; j < get_columns(); ++j )
		{
			m_data[ row ][ j ] /= scalar;
		}
	}

	void
	row_divisor_column ( unsigned row, unsigned column_begin, 
		unsigned column_end, double scalar )
	{
		assert( scalar != 0 );
		for ( unsigned j = column_begin; j < column_end; ++j )
		{
			m_data[ row ][ j ] /= scalar;
		}
	}

	int
	find_nonzero ( unsigned start_row, unsigned col )
	{
		unsigned curr_row;
		for ( curr_row = start_row; 
			( curr_row < get_rows()) && 
			( m_data[ curr_row ][ col ] == 0 ); ++curr_row )
		{ }

		if ( curr_row == get_rows() ) 
		{
			return -1;
		}
	
		if ( curr_row > start_row )
		{
			swap_rows( curr_row , start_row );
		}
				
		if ( m_data[ start_row ][ col ] != 1 )
		{
			row_divisor( curr_row , m_data[ start_row ][ col ] );
		}
		return start_row;
	}

	int
	find_nonzero_column ( unsigned start_row, unsigned col )
	{
		unsigned curr_row;
		for ( curr_row = start_row; 
			( curr_row < get_rows()) && 
			( m_data[ curr_row ][ col ] == 0 ); ++curr_row )
		{ }

		if ( curr_row == get_rows() ) return -1;

		return curr_row;
	}

private:

	vector<vector<double>> m_data;
};

/************************************************************/

ostream&
operator<< ( ostream& os, Matrix matrix )
{
	matrix.fix_sign_zeroes( );
	os << endl;
	for ( unsigned i = 0; i < matrix.get_rows(); ++i )
	{
		for ( unsigned j = 0; j < matrix.get_columns(); ++j )
		{
			os << matrix[ i ][ j ] << "\t";
		}
		os << endl;
	}
	os << endl;
	return os;
}

/************************************************************/

void
generate_matrix ( Matrix& matrix )
{
	random_device rd;
    mt19937 gen( rd( ) );
    gen.seed( 0 );
  	uniform_int_distribution<int> dist( -20, 20 );

  	for ( unsigned i = 0; i < matrix.get_rows(); ++i )
  	{
  		generate_n ( matrix[ i ].begin (), matrix.get_columns(), 
  			[&gen, &dist] () { return dist (gen); });
  	}
}

/************************************************************/

void
initalize_test_matrix ( Matrix& matrix )
{
	cout << endl;
	for ( unsigned i = 0; i < matrix.get_rows(); ++i )
	{
		for ( unsigned j = 0; j < matrix.get_columns(); ++j )
		{
			cout << "Matrix [ " << i << " ][ " << j << " ] ==> ";
			cin >> matrix[ i ][ j ];
		}
	}
	cout << endl;
}

/************************************************************/

#endif