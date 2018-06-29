/*
  Filename   : SearchTree.hpp
  Author     : Zachary Zuch
  Course     : CSCI 362-01
  Description: Binary search tree class, 
              a basis for implementing associative containers. 
*/

/************************************************************/
// Macro guard

#ifndef SEARCHTREE_HPP
#define SEARCHTREE_HPP

/************************************************************/
// System includes

#include <iostream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <queue>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

using std::cout;
using std::endl;
using std::ostream;
using std::ostream_iterator;
using std::queue;

/************************************************************/

template<typename T>
struct Node
{
  typedef Node* NodePtr;

  // Initialize data, left, right, and parent in
  //   the member initialization list
  // left, right, and parent should be set to nullptr
  // The body of this constructor should be empty
  Node (const T& d = T ())
    : data ( d ), left ( nullptr ), right ( nullptr ), parent ( nullptr )
  { }

  // Initialize data, left, right, and parent in
  //   the member initialization list
  // The body of this constructor should be empty
  Node (const T& d, NodePtr l, NodePtr r, NodePtr p)
    : data ( d ), left ( l ), right ( r ), parent ( p )
  { }

  T        data;
  NodePtr  left;
  NodePtr  right;
  NodePtr  parent;

};

/************************************************************/
// Forward declaration

template<typename T>
class SearchTree;

/************************************************************/

template<typename T>
struct TreeIterator
{
  friend class SearchTree<T>;

  typedef TreeIterator<T>  Self;
  typedef Node<T>*         NodePtr;
  typedef const Node<T>*   ConstNodePtr;

  typedef T                value_type;
  typedef const T*         pointer;
  typedef const T&         reference;

  TreeIterator ()
    : m_nodePtr ()
  { }

  explicit
  TreeIterator (ConstNodePtr n)
    : m_nodePtr (n)
  { }

  // Return the data associated with the node
  reference
  operator* () const
  {
    return m_nodePtr->data;
  }

  // Return address of node's data member
  pointer
  operator-> () const
  {
    return &*this;
  }

  // Pre
  Self&
  operator++ ()
  {
    m_nodePtr = increment (m_nodePtr);
    return *this;
  }

  // Post
  // Increment iterator so it points to in-order successor
  Self
  operator++ (int)
  {
    Self temp (*this);
    m_nodePtr = increment (m_nodePtr);
    return temp;
  }

  // Pre
  Self&
  operator-- ()
  {
    m_nodePtr = decrement (m_nodePtr);
    return *this;
  }

  // Post
  // Decrement iterator so it points to in-order predecessor
  Self
  operator-- (int)
  {
    Self temp (*this);
    m_nodePtr = decrement (m_nodePtr);
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

private:

  // Find in-order successor of "n"
  static ConstNodePtr
  increment (ConstNodePtr n)
  {
    if ( n == nullptr )
    {
      return nullptr;
    }
    else if ( n->parent == nullptr )
    {
      return n;
    }
    else if ( n->right != nullptr )
    {
      ConstNodePtr curr = n->right;
      while ( curr->left != nullptr ) 
      {
        curr = curr->left;
      }
      return curr;
    }
    else
    {
      ConstNodePtr curr = n;
      while (( curr->data > curr->parent->data ) && ( curr != curr->parent->parent ))
      {
        curr = curr->parent;
      }
      return curr->parent;
    }
  }

  // Find in-order predecessor of "n"
  static ConstNodePtr
  decrement (ConstNodePtr n)
  {
    if ( n == nullptr )
    {
      return nullptr;
    }
    else if ( n->parent == nullptr )
    {
      return n;
    }
    else if ( n->left != nullptr )
    {
      ConstNodePtr curr = n->left;
      while ( curr->right != nullptr ) 
      {
        curr = curr->right;
      }
      return curr;
    }
    else
    {
      ConstNodePtr curr = n;
      while (( curr->data < curr->parent->data ) && ( curr != curr->parent->parent ))
      {
        curr = curr->parent;
      }
      return curr->parent;
    }
  }

private:

  ConstNodePtr m_nodePtr;

};

/************************************************************/

template<typename T>
class SearchTree
{
  friend class TreeIterator<T>;

public:

  typedef T             value_type;
  typedef T*            pointer;
  typedef const T*      const_pointer;
  typedef T&            reference;
  typedef const T&      const_reference;

  typedef TreeIterator<T> iterator;
  typedef TreeIterator<T> const_iterator;

  // Header parent points to root of tree or is nullptr
  //   if the tree is empty
  // Header left points to LARGEST node or is nullptr
  //   if the tree is empty  
  // Header right points to SMALLEST node or is nullptr
  //   if the tree is empty
  SearchTree ()
    : m_header (), m_size (0)
  {
  }

  ~SearchTree ()
  {
    clear ();
  }

  bool
  empty () const
  {
    return ( m_size == 0 );
  }

  size_t
  size () const
  {
    return m_size;
  }

  int
  depth () const
  {
    return depth( m_header.parent );
  }

  // Build an iterator that points to the smallest element
  iterator
  begin ()
  {
    iterator itr ( m_header.right );
    return itr;
  }

  // Build an iterator that points to the smallest element
  const_iterator
  begin () const
  {
    const_iterator itr ( m_header.right );
    return itr;
  }

  // Build an iterator that points to one beyond the end
  // (i.e., the head node)
  iterator
  end ()
  {
    iterator itr ( &m_header );
    return itr;
  }

  // Build an iterator that points to one beyond the end
  // (i.e., the head node)
  const_iterator
  end () const
  {
    const_iterator itr ( &m_header );
    return itr;
  }

  iterator
  find (const T& v) const
  {
    ConstNodePtr n = findHelper (v);
    if (n == nullptr)
    {
      // Wasn't found so want to return end ()
      n = &m_header;
    }
    return iterator (n);
  }

  std::pair<iterator, bool>
  insert (const T& v)
  {
    NodePtr insertedNode = insert (v, m_header.parent, &m_header);
    bool inserted = ( insertedNode != nullptr );
    if ( inserted )
    {
      // Update header right to point to smallest node
      if ( m_header.right == nullptr || v < m_header.right->data )
      {
        m_header.right = insertedNode;
      }
      // Update header left to point to largest node
      if ( m_header.left == nullptr || v > m_header.left->data )
      {
        m_header.left = insertedNode;
      }
    }

    return std::make_pair (iterator (insertedNode), inserted);
  }

  size_t
  erase (const T& v)
  {
    // Update header right to point to smallest node
    if ( m_header.right != nullptr && v == m_header.right->data )
    {
      m_header.right =
        const_cast<NodePtr> (iterator::increment (m_header.right));
    }
    // Update header left to point to largest node
    if ( m_header.left != nullptr && v == m_header.left->data )
    {
      m_header.left =
        const_cast<NodePtr> (iterator::decrement (m_header.left));
    }
    bool erased = erase (v, m_header.parent, &m_header);
    return erased ? 1 : 0;
  }

  void
  clear ()
  {
    clear ( m_header.parent );
    m_header.parent = nullptr;
  }

  void
  printInorder (ostream& out) const
  {
    printInorder (out, m_header.parent);
  }

  void
  printLevelorder (ostream& out) const
  {
    printLevelorder (out, m_header.parent);
  }

private:

  typedef struct Node<T> Node;
  typedef Node*          NodePtr;
  typedef const Node*    ConstNodePtr;

  int
  depth ( NodePtr r ) const
  {
    if ( r == nullptr ) 
      return -1;

    else if ( r->left == nullptr && r->right == nullptr ) 
      return 0;

    else
    {
      int leftDepth = 0;
      int rightDepth = 0;

      if ( r->left != nullptr )
        leftDepth = depth ( r->left ) + 1;
      if ( r->right != nullptr )
        rightDepth = depth ( r->right ) + 1;

      return ( leftDepth > rightDepth ) ? leftDepth : rightDepth;
    }
  }

  NodePtr
  minimum (NodePtr r) const
  {
    NodePtr curr = r;
    for ( ; curr->left != nullptr; curr = curr->left ) { }
    return curr;
  }

  NodePtr
  maximum (NodePtr r) const
  {
    NodePtr curr = r;
    for ( ; curr->right != nullptr; curr = curr->right ) { }
    return curr;
  }
  
  // NEED TO WORK ON
  ConstNodePtr
  findHelper (const T& v) const
  {
    // Return a pointer to the node that contains "v"
    ConstNodePtr curr = m_header.parent;
    while (( curr != nullptr ) && ( curr->data != v ))
    {
      curr = ( v < curr->data ) ? curr->left : curr->right;
    }
    return curr;
  }

  // Insert "v" into the tree rooted at "r"
  // Use "parent" for recursion and setting the parent of the
  //   node containing "v"
  NodePtr
  insert (const T& v, NodePtr& r, NodePtr parent)
  {
    if ( r == nullptr )
    {
      r = new Node ( v , nullptr , nullptr , parent );
      ++m_size;
      return r;
    }
    else if ( v < r->data )
    {
      return insert ( v , r->left , r );
    }
    else if ( v > r->data )
    {
      return insert ( v , r->right , r );
    }
    else
    {
      return nullptr;
    }
  }

  // Erase "v" from the tree rooted at "r"
  // Use "parent" for recursion and updating the parent of the
  //   node containing "v"
  // Return whether the erase succeeded or not
  bool
  erase (const T& v, NodePtr& r, NodePtr parent)
  {
    if ( r == nullptr )
    {
      return false;
    }
    else if ( v < r->data )
    {
      return erase( v, r->left, r );
    }
    else if ( v > r->data )
    {
      return erase( v, r->right, r );
    }
    else if ( r->left != nullptr && r->right != nullptr )
    {
      NodePtr ioSucc = minimum( r->right );
      r->data = ioSucc->data;
      return erase( r->data, r->right, r );
    }
    else
    {
      NodePtr succ = ( r->left != nullptr ) ? r->left : r->right;
      NodePtr tmp = r;
      r = succ;
      if ( succ != nullptr )
      {
        succ->parent = parent;
      }
      delete tmp;
      --m_size;
      return true;
    }
  }

  void
  clear (NodePtr r)
  {
    if ( r == nullptr ) return;

    clear ( r->left );
    clear ( r->right );
    --m_size;
    delete r;
  }

  void
  printInorder (ostream& out, NodePtr r) const
  {
    if (r != nullptr)
    {
      printInorder (out, r->left);
      out << r->data << " ";
      printInorder (out, r->right);
    }
  }

  // NEED TO WORK ON
  // FIXME: This routine has bugs
  // Fix the bugs and modify this method to
  //   output elements in the form required by the
  //   operator<< below. 
  void
  printLevelorder (ostream& out, NodePtr r) const
  {
    if ( r == nullptr ) return;

    queue<NodePtr> q;
    NodePtr lastEle = const_cast<NodePtr> ((--end()).m_nodePtr);
    size_t numNodes = 0;
    size_t maxNodes = 1;
    int level = 0;
    int d = depth();

    q.push (r);
    while ( !q.empty() )
    {
      r = q.front( );
      q.pop( );

      if ( r != nullptr )
      {
        if ( r->parent == nullptr )
        {
          out << "- ";
        }
        else
        {
          out << r->data << " ";
        }
        if ( r->left == nullptr && d > level + 1 )
        {
          q.push( new Node( ) );
        }
        else
        {
          q.push( r->left );
        }
        if ( r->right == nullptr && d > level + 1 )
        {
          q.push( new Node( ) );
        }
        else
        {
          q.push( r->right );
        }
      }
      else
      {
        out << "- ";
      }
      ++numNodes;

      if ( numNodes == maxNodes && level == d ) return;

      if ( numNodes == maxNodes )
      {
        numNodes = 0;
        ++level;
        maxNodes *= 2;

        out << "| ";
      }
    }
  }

private:

  Node   m_header;
  size_t m_size;

};

/************************************************************/

// Output tree as ( e1 e2 e3 ... en )
//   with '|' to indicate the end of a level, and '-' to indicate
//   a missing node.
// E.g., the tree
//       4
//    2     7
//         6
// should be printed thus: ( 4 | 2 7 | - - 6 ) 
template<typename T>
ostream&
operator<< (ostream& out, const SearchTree<T>& tree)
{
  out << "( ";
  // tree.printInorder (out);
  tree.printLevelorder (out);
  out << ")";

  return out;
}

/************************************************************/

#endif

/************************************************************/
