/*
  Filename   : Array.hpp
  Author     : Zachary Zuch
  Course     : CSCI 362-02
  Assignment : Lab 2
  Description: Array class, my implementation of a list ADT 
                 with random access and dynamic resizing.
*/   

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef ARRAY_H
#define ARRAY_H

/************************************************************/
// System includes

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>

/************************************************************/
// Using declarations

using std::swap;
using std::fill;
using std::copy;
using std::copy_backward;
using std::ostream;
using std::ptrdiff_t;
using std::distance;

/************************************************************/

template <typename T>
class Array 
{
public:
  //*****************************************************
  // DO NOT MODIFY THIS SECTION OF typedef's
  // Some standard typedef's
  typedef T          value_type;
  // Iterators are just pointers to objects of type T
  typedef T*         iterator;
  typedef const T*   const_iterator;

  typedef T&         reference;
  typedef const T&   const_reference;
  typedef size_t     size_type;
  typedef ptrdiff_t  difference_type;
  //*****************************************************
  
  // Initialize an empty Array
  Array ()
    : m_size(0), 
      m_capacity(0), 
      m_array(new T[0])
  {
  }

  // Initialize an Array of size "pSize", with each element
  //   set to "value"
  explicit Array (size_t pSize, const T& value = T ())
    : m_size(pSize), 
      m_capacity(pSize), 
      m_array(new T[pSize])
  {
    fill(begin(), end(), value);
  }

  // Initialize an Array from the range [first, last)
  Array (const_iterator first, const_iterator last)
    : m_size(last - first), 
      m_capacity(last - first), 
      m_array(new T[last - first])
  {
    copy(first, last, m_array);
  } 
  
  // Initialize this object from "a"
  Array (const Array& a)
    : m_size(a.m_size), 
      m_capacity(a.m_capacity), 
      m_array(new T[a.m_capacity])
  {
    copy(a.begin(), a.end(), m_array);
  }
  
  // Release allocated memory
  ~Array ()
  {
    delete[] m_array;
  }
  
  // Assign "a" to this object
  Array&
  operator= (const Array& a)
  {
    m_size = a.m_size;
    m_capacity = a.m_capacity; 

    copy(a.begin(), a.end(), m_array);
  }

  // Return the size
  size_t
  size () const
  {
    return m_size;
  }

  // Return the capacity
  size_t
  capacity () const
  {
    return m_capacity;
  }

  // Return the element at position "index"
  T&
  operator[] (size_t index)
  {
    return m_array[index];
  }

  const T&
  operator[] (size_t index) const
  {
    return m_array[index];
  }

  // Insert an element at the back
  void
  push_back (const T& item)
  {
    if (m_size == m_capacity)
    {
      reserve( (m_capacity + 1) * 2 );
    }
    m_array[ m_size ] = item;
    ++m_size;
  }

  // Erase the last element
  void
  pop_back ()
  {
    --m_size;
  }

  // Reserve capacity for "space" elements
  // "space" must be  > capacity
  //   if not, leave the capacity unchanged
  // "size" is unchanged
  void
  reserve (size_t space)
  {
    if (space > m_capacity)
    {
      T* arr = new T[space];
      copy(begin(), end(), arr);
      delete[] m_array;
      m_array = arr;
      m_capacity = space;
    }
  }

  // Change the size to be "newSize"
  // If "newSize" is less than "size",
  //   erase the last elements
  // If "newSize" is more than "size", 
  //   insert "defValue"-s at the end
  void
  resize (size_t newSize, const T& defValue = T ())
  {
    if (newSize > m_capacity)
    {
      T* arr = new T[newSize];
      copy(begin(), end(), arr);
      fill(arr + m_capacity, arr + newSize, defValue);
      delete[] m_array;
      m_array = arr;
      m_capacity = newSize;
    }
    m_size = newSize;
  }

  // Insert "item" before "pos", and return iterator pointing to "item"
  // If the capacity is insufficient, double it
  iterator
  insert (iterator pos, const T& item)
  {
    if (m_size == m_capacity)
    {
      int index = pos - m_array;
      reserve((m_capacity + 1) * 2);
      pos = m_array + index;
    }
    copy_backward(pos, end(), end() + 1);
    (*pos) = item;
    ++m_size;
    return pos;
  }

  // Remove element at "pos", and return iterator
  //   referencing next element
  iterator
  erase (iterator pos)
  {
    copy(pos + 1, end(), pos);
    --m_size;
    return pos;
  }

  void
  bubbleSort ()
  {
    bool didSwap;
    for (size_t i = m_size - 1; i > 0; --i)
    {
      didSwap = false;
      for (size_t j = 0; j < i; ++j)
      {
        if (m_array[j] > m_array[j + 1])
        {
          swap(m_array[j], m_array[j + 1]);
          didSwap = true;
        }
      }
      if (!didSwap) break;
    }
  }

  void 
  insertionSort ()
  {
    T ele;
    size_t j;
    for (size_t i = 1; i < m_size; ++i)
    {
      ele = m_array[i];
      for (j = i; (j >= 1) && (ele < m_array[j - 1]); --j)
      {
        m_array[j] = m_array[j - 1];
      }
      m_array[j] = ele;
    }
  }

  void
  selectionSort ()
  {
    size_t min;
    size_t j;
    for (size_t i = 0; i < m_size - 1; ++i)
    {
      min = i;
      for (j = i + 1; j < m_size; ++j)
      {
        if (m_array[j] < m_array[min]) 
        {
          min = j;
        }
      }
      swap(m_array[i], m_array[min]);
    }
  }

  // Return iterator pointing to first element
  iterator
  begin ()
  {
    return m_array;
  }

  const_iterator
  begin () const
  {
    return m_array;
  }

  // Return iterator pointing one beyond the last element
  iterator
  end ()
  {
    return m_array + m_size;
  }

  const_iterator
  end () const
  {
    return m_array + m_size;
  }

private:
  size_t m_size;
  size_t m_capacity;
  T*     m_array;
};

/************************************************************************/
// Free functions associated with the class

// Output operator
// Allows us to do "cout << a;", where "a" is an Array
template <typename T>
ostream&
operator<< (ostream& output, const Array<T>& a)
{
  output << "[ ";
  for (const auto& elem : a)
  {
    output << elem << " ";
  }
  output << "]";

  return output;
}

#endif
/************************************************************************/
