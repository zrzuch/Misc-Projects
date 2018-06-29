/*
  Filename   : List.hpp
  Author     : Zachary Zuch
  Course     : CSCI 362
  Assignment : Lab 5
  Description: List class, our implementation of a list ADT 
                 using a doubly-linked list data structure.
*/

/************************************************************/
// Macro guard

#ifndef LIST_HPP
#define LIST_HPP

/************************************************************/
// System includes

#include <iostream>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <initializer_list>

/************************************************************/
// Using declarations

using std::ostream;
using std::ostream_iterator;
using std::initializer_list;

/************************************************************/

template <typename T>
struct ListNode 
{
  ListNode ( )
    : data( ), next( this ), prev( this )
    // Make a circular node, with next and prev
    //   pointing to this node
  {
  }

  ListNode (const T& d, ListNode* n = nullptr,
	    ListNode* p = nullptr)
  : data( d ), next( n ), prev( p )
    // Initialize the data members
  {
  }

  T          data;
  ListNode*  next;
  ListNode*  prev;
};

/************************************************************/

template<typename T>
struct ListIterator
{
  typedef ListIterator<T>                  Self;
  typedef ListNode<T>                      Node;

  typedef ptrdiff_t                        difference_type;
  typedef std::bidirectional_iterator_tag  iterator_category;

  typedef T                                value_type;
  typedef T*                               pointer;
  typedef T&                               reference;

  ListIterator ()
    : m_nodePtr ()
  { }

  explicit
  ListIterator (Node* n)
    : m_nodePtr (n) 
  { }
  
  reference
  operator* () const
  { 
    return m_nodePtr->data;
  }
  
  // Return address of node's data member
  pointer
  operator-> () const
  { 
    return &m_nodePtr->data;
  }
  
  // Pre-increment
  Self&
  operator++ ()
  {
    m_nodePtr = m_nodePtr->next;
    return *this;
  }
  
  // Post-increment
  Self
  operator++ (int)
  {
    Self temp (*this);
    m_nodePtr = m_nodePtr->next;
    return temp;
  }

  // Pre-decrement
  Self&
  operator-- ()
  {
    m_nodePtr = m_nodePtr->prev;
    return *this;
  }
  
  // Post-decrement
  Self
  operator-- (int)
  {
    Self temp (*this);
    m_nodePtr = m_nodePtr->prev;
    return temp;
  }
  
  bool
  operator== (const Self& i) const
  {
    return m_nodePtr == i.m_nodePtr;
  }
  
  bool
  operator!= (const Self& i) const
  {
    return m_nodePtr != i.m_nodePtr;
  }
  
  /************************************************************/

  Node* m_nodePtr;

};

/************************************************************/

template<typename T>
struct ListConstIterator
{
  typedef ListConstIterator<T>     Self;
  typedef const ListNode<T>        Node;
  typedef ListIterator<T>          iterator;
  
  typedef ptrdiff_t                        difference_type;
  typedef std::bidirectional_iterator_tag  iterator_category;

  typedef       T                   value_type;
  typedef const T*                  pointer;
  typedef const T&                  reference;
  
  ListConstIterator ()
    : m_nodePtr ()
  { }
  
  explicit
  ListConstIterator (Node* n)
    : m_nodePtr (n)
  { }
  
  ListConstIterator (const iterator& i)
    : m_nodePtr (i.m_nodePtr)
  { 
  }
  
  reference
  operator* () const
  { 
    return m_nodePtr->data;
  }
  
  pointer
  operator-> () const
  { 
    return &m_nodePtr->data;
  }

  Self&
  operator++ ()
  {
    m_nodePtr = m_nodePtr->next;
    return *this;
  }
  
  Self
  operator++ (int)
  {
    // post-increment
    Self temp (*this);
    m_nodePtr = m_nodePtr->next;
    return temp;
  }
  
  Self&
  operator-- ()
  {
    m_nodePtr = m_nodePtr->prev;
    return *this;
  }
  
  Self
  operator-- (int)
  {
    // post-decrement
    Self temp (*this);
    m_nodePtr = m_nodePtr->prev;
    return temp;
  }
  
  bool
  operator== (const Self& i) const
  { 
    return m_nodePtr == i.m_nodePtr;
  }
  
  bool
  operator!= (const Self& i) const
  { 
    return m_nodePtr != i.m_nodePtr;
  }
  
  /************************************************************/

  Node* m_nodePtr;

};
// Return whether the two iterators refer to the same node
template<typename T>
inline bool
operator== (const ListIterator<T>& i1,
	    const ListConstIterator<T>& i2)
{
  return ( i1.m_nodePtr == i2.m_nodePtr );
}

// Return whether the two iterators refer to different nodes
template<typename T>
inline bool
operator!= (const ListIterator<T>& i1,
	    const ListConstIterator<T>& i2)
{ 
  return ( i1.m_nodePtr != i2.m_nodePtr );
}

/************************************************************/

template<typename T>
class List 
{
  typedef ListNode<T>            Node;

public:

  typedef ListIterator<T>        iterator;
  typedef ListConstIterator<T>   const_iterator;
  typedef T                      value_type;
  typedef T*                     pointer;
  typedef T&                     reference;
  typedef const T&               const_reference;

  List ()
  : m_header(), m_size(0)
  // Initialize data members
  {
  }

  // Add all values to the list
  List (initializer_list<T> values)
    : m_header (), m_size (0)
  {
    for ( typename initializer_list<T>::iterator it = values.begin(); 
      it != values.end(); ++it )
    {
      push_back( *it );
    }
  }  

  // Initialize a list of size "n", with each value set to "defValue"
  explicit List (size_t n, const T& defValue = T ())
    : m_header( ), m_size( 0 )
  {
    for ( size_t i = 0; i < n; ++i )
    {
      push_front(defValue);
    }
  }

  // Copy constructor
  //   Initialize this list to be a copy of "otherList"
  List (const List& otherList)
    : m_header( ), m_size( otherList.m_size )
  {
    for ( const_iterator curr = otherList.begin(); 
      curr != otherList.end(); ++curr )
    {
      push_back( *curr );
    }
  }

  ~List ()
  {
    erase( begin() , end() );
  }

  List&
  operator= (const List& rhs)
  {
    // Assign "rhs" to this list
    if (!empty())
    {
      erase( begin() , end() );
    }

    for ( const_iterator curr = rhs.begin(); 
      curr != rhs.end(); ++curr )
    {
      push_back( *curr );
    }

    return *this;
  }
  
  bool    
  empty () const
  {
    return ( m_size == 0 );
  }
  
  size_t
  size ()  const
  {
    return m_size;
  }

  // Return the first element
  reference
  front ()
  {
    return m_header.next->data;
  }

  const_reference
  front () const
  {
    return m_header.next->data;
  }

  // Return the back element
  reference
  back ()
  {
    return m_header.prev->data;
  }

  const_reference
  back () const
  {
    return m_header.prev->data;
  }

  // Add "item" to the front of the list
  void 
  push_front (const_reference item)
  {
    insert( begin(), item );
  }

  // Remove the first element
  void
  pop_front  ( )
  {
    erase( begin() );
  }

  // Add "item" to the back of the list
  void 
  push_back  (const_reference item)
  {
    insert( end(), item );
  }

  // Remove the last element
  void 
  pop_back   ( )
  {
    erase( --end() );
  }

  // Return an iterator referencing the first element
  iterator       
  begin ( )
  {
    iterator i ( m_header.next );
    return i;
  }

  const_iterator 
  begin ( ) const
  {
    const_iterator i ( m_header.next );
    return i;
  }

  // Return an iterator referencing one past the last element
  iterator       
  end   ( )
  {
    iterator i ( &m_header );
    return i;
  }

  const_iterator 
  end   ( ) const
  {
    const_iterator i ( &m_header );
    return i;
  }  

  // Insert "item" at position "i"
  // Return an iterator referencing the inserted element
  iterator 
  insert (iterator i, const T& item)
  {
    Node* succ = i.m_nodePtr;
    Node* pred = i.m_nodePtr->prev;
    Node* newNode = new Node(item, succ, pred);
    pred->next = newNode;
    succ->prev = newNode;
    ++m_size;
    --i;
    return i;
  }

  // Erase element at position "i"
  // Return an iterator referencing the element beyond the one erased
  iterator 
  erase  (iterator i)
  {
    Node* n = i.m_nodePtr;
    ++i;
    n->prev->next = n->next;
    n->next->prev = n->prev;
    --m_size;
    delete n;
    return i;
  } 

  // Erase elements in the range [i1, i2)
  // Return iterator "i2"
  iterator 
  erase  (iterator i1, iterator i2)
  {
    iterator curr = i1;
    while ( curr != i2 )
    {
      curr = erase( curr );
    }
    return curr;
  }
  
private:
  // Dummy header
  Node   m_header;
  size_t m_size;

};

/************************************************************/

// output "list" as ( e1 e2 e3 ... en )
template<typename T>
ostream&
operator<< (ostream& out, const List<T>& list)
{
  out << "( ";
  for ( ListConstIterator<T> it = list.begin(); it != list.end(); ++it )
  {
    out << *( it ) << " ";
  }
  out << ')';
  return out;
}

/************************************************************/

#endif

/************************************************************/
