#ifndef MATRIX_HPP
#define MATRIX_HPP

template <typename T>
class Matrix   
{

public:

  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using const_reference = const T&;

  // Initialize a matrix of size 'numRows' x 'numCols'
  Matrix ( unsigned numRows , unsigned numCols )
    : m_numRows ( numRows ) , 
      m_numCols ( numCols ) , 
      m_data ( new value_type[ m_numRows * m_numCols ] )
  {}

  // Copy constructor
  Matrix ( const Matrix& m )
    : m_numRows ( m.m_numRows ) , 
      m_numCols ( m.m_numCols ) , 
      m_data ( new value_type[ m_numRows * m_numCols ] )
  { 
    for ( int i = 0; i < m_numRows * m_numCols; ++i )
      m_data[ i ] = m.m_data[ i ];
  }

  // Destructor
  ~Matrix ()
  {
    delete [] m_data;
  }

  unsigned
  getRows( )
  {
    return m_numRows;
  }

  unsigned
  getColumns( )
  {
    return m_numCols;
  }

  // Copy assignment
  Matrix&
  operator= ( const Matrix& m )
  {
    m_numRows = m.m_numRows;
    m_numCols = m.m_numCols;
    m_data = new value_type[ m_numRows * m_numCols ];

    for ( int i = 0; i < m_numRows * m_numCols; ++i ) 
      m_data[ i ] = m.m_data[ i ];

    return ( *this );
  }

  // Subscript operator (non-const)
  // Do NOT do bounds checking
  reference
  operator () ( unsigned row, unsigned col )
  {
    return m_data[ ( row * m_numCols ) + col ];
  }

  // Subscript operator (const)
  // Do NOT do bounds checking
  const_reference
  operator () ( unsigned row, unsigned col ) const
  {
    return m_data[ ( row * m_numCols ) + col ];
  }

private:

  unsigned m_numRows;
  unsigned m_numCols;
  pointer m_data;

};

#endif