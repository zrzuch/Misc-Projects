#ifndef _LBVECTOR_H
#define _LBVECTOR_H

#include <cstdlib>
#include <cassert>
#include <iostream>

// templated vector class, partially based on Budd,
//                         Classic Data Structures in C++
// written 11/5/93, modified 3/23/94
// changed on 3/9/95 to make all methods inline (defined in class decl)
// changed on 10/14/99 to clean up for vector lab in CS 330
// changed on 10/14/04 to update for C++ changes
//////////////////////////////////////////////////////////////////////////
// updated 10/23/15 by Zachary Zuch to allow lower and upper bounds
//////////////////////////////////////////////////////////////////////////
//
// for a vector of Items use lbvector<Item>,
//                   e.g., lbvector <int> intvector;
//                   note: Item must have a default constructor
//
// constructors:
//   lbvector( )             -- default, vector of size 0 (no entries)
//   lbvector(int size)      -- vector with size entries
//   lbvector(int lowerBound,
//          int upperBound)  -- vector w/ indices from the lower bound
//                              to the upper bound
//   lbvector(int lowerBound,
//          int upperBound,
//          Item fillValue)  -- vector w/ indices from the lower bound
//                              to the upper bound that has every
//                              index filled with the determined value
//
//   lbvector(const lbvector & vec) 
//                           -- copy constructor
//    
//   int capacity( )         -- returns capacity of the vector
//
//   int lower( )            -- returns the lower bound of the vector
// 
//   int upper( )            -- returns the upper bound of the vector
// 
//   void resize(int newSize) 
//                           -- resizes the vector to accomendate a new 
//                              lower and upper bound
//                              (can result in losing elements if
//                                   new size < old size)
//
//   void fill(Item fillValue)
//                           -- set all entries equal to fillValue
//
//   operator =              -- assignment operator works properly
//   operator []             -- indexes both const and non-const vectors
//    
//  examples of use:
//     lbvector<double> dlist(100);      // a vector of 100 doubles
//     lbvector<double> dzlist(-3 , 3);  // a vector of 7 doubles from
//                                          -3 to 3
//
//     lbvector<String> slist(300);      // 300 strings
//
//     lbvector<int> ilist;              // has room for 0 ints
//
//
//  class invariant:
//   - myList points to the beginning of the allocated space
//   - myZero points to the logical zero of the array to allow for
//     indexing of any integer number
//   - lowerb is the lower bound of the vector
//   - upperb is the upper bound of the vector
//   - myCapacity is the number of elements allocated


template <class Item> class lbvector
{
public:
   // default constructor 0 elements
   // postcondition: vector of zero items constructed
   lbvector( ) 
   {
      myCapacity = 0;
      lowerb = 0;
      upperb = 0;

      myList = 0;
      myZero = myList;
   }
    
   // specify size of vector
   // postcondition: vector of size items constructed   
   lbvector(int size) 
   {
      assert(size >= 0);

      myCapacity = size;
      lowerb = 0;
      upperb = size - 1;

      myList = new Item [size];
      myZero = myList; 

      assert(myList != 0);
   }
    
   /*
   // specify size and fill value
   // postcondition: constructed vector of size items,
   //                each item initialized to fillValue
   lbvector(int size, Item fillValue) 
   {
      assert(size >= 0);

      myCapacity = size;
      lowerb = 0;
      upperb = size - 1;

      myList = new Item [size];
      myZero = myList;

      assert(myList != 0);
      fill(fillValue);
   }
   */

   // copy constructor
   // used to create a new and complete copy of the object (vec)
   //    as when passing the object by value or returning object 
   //    as a result - need a deep copy when using dynamic memory
   // precondition: Item supports assignment
   // postcondition: constructed vector is an exact duplicate of vec        
   lbvector(const lbvector<Item> & vec) 
   {
      // allocate storage
      myCapacity = vec.myCapacity;
      lowerb = vec.lowerb;
      upperb = vec.upperb;

      myList = new Item [myCapacity];
      myZero = myList - lowerb;
        
      // copy elements from vec
      for (int k = 0; k < vec.myCapacity; k++) 
      {
         myList[k] = vec.myList[k];
      }       
   }

   // specify the upper and lower bound of a vector
   // used to create a new vector that allows for any possible
   //    integer upper and lower bound for the indices of the
   //    vector
   // precondition: lowerBound <= upperBound
   lbvector(int lowerBound, int upperBound)
   {
      assert(lowerBound <= upperBound);

      myCapacity = upperBound - lowerBound + 1;
      lowerb = lowerBound;
      upperb = upperBound;

      myList = new Item [myCapacity];
      myZero = myList - lowerb;
   }

   // specify the upper bound, lower bound, and the value to fill in
   // used to create a new vector that allows for any possible
   //    integer as the upper and lower bound for the indices of the
   //    vector, and it will fill in the vector with the given value
   // precondition: lowerBound <= upperBound
   lbvector(int lowerBound, int upperBound, Item fillValue)
   {
      assert(lowerBound <= upperBound);

      myCapacity = upperBound - lowerBound + 1;
      lowerb = lowerBound;
      upperb = upperBound;
      
      myList = new Item [myCapacity];
      myZero = myList - lowerb;

      fill(fillValue);
   }
    
   // free new'd storage
   // postcondition: dynamically allocated storage freed
   ~lbvector ( ) 
   {
      myZero = 0;
      delete [] myList;
   }

   // assignment
   //    need a deep copy when using dynamic memory
   // precondition: Item supports assignment     
   // postcondition: self is assigned vec
   lbvector & operator = (const lbvector<Item> & vec) 
   {
      // don't assign to self!
      if (this != &vec) 
      {
         // out with old list, in with new
         delete [] myList;

         myCapacity = vec.myCapacity;
         lowerb = vec.lowerb;
         upperb = vec.upperb;

         myList = new Item [myCapacity];
         myZero = myList - lowerb;

         assert(myList != 0);
            
         // copy elements from vec
         for (int k = 0; k < vec.myCapacity; k++) 
         {
            myList[k] = vec.myList[k];
         }
      }
      return *this;           
   }

   // change size dynamically
   // precondition: vector has room for myCapacity entries     
   // postcondition: vector has room for newSize entries
   //          the first myCapacity of which are copies of original
   //          unless newSize < myCapacity, then truncated copy occurs
   void resize(int lowerBound, int upperBound) 
   {
      // allocate new storage
      int newCapacity = (upperBound - lowerBound + 1);
      Item * newList = new Item[newCapacity];
      assert(newList != 0);
      Item * zero = newList - lowerBound;

      // copy retained values from old to new
      if (myCapacity > 0)
      {
         int start = lowerb > lowerBound ? lowerb : lowerBound;
         int stop = upperb < upperBound ? upperb : upperBound;
         
         for ( int k = start; k <= stop; k++ )
         {
            *( zero + k ) = *( myZero + k );
         }
      }
      
      // return space no longer needed       
      delete [] myList;

      // update instance variables to reflect changes
      lowerb = lowerBound;
      upperb = upperBound;
      myCapacity = newCapacity;

      myList = newList;
      myZero = zero;
   }
    
   // capacity of vector
   int capacity( ) const 
   {
      return myCapacity;
   }

   // lower bound of vector
   int lower( ) const
   {
      return lowerb;
   }

   // upper bound of vector
   int upper( ) const 
   {
      return upperb;
   }

   // Fills the vector with the value of the parameter
   // postcondition: all entries are equal to fillValue
   void fill(Item fillValue) 
   {
      for (int k = 0; k < myCapacity; k++) 
      {
         myList[k] = fillValue;
      }
   }

   // safe indexing, returning reference
   // precondition: 0 <= index < myCapacity
   // postcondition: return index-th item
   // exception: aborts if index is out-of-bounds
   Item & operator [] (int index) 
   {
      checkIndex ( index );
      return *( myZero + index );     
   }
    
   // const index 
   // safe indexing, returning const reference to avoid modification
   // precondition: 0 <= index < myCapacity
   // postcondition: return index-th item
   // exception: aborts if index is out-of-bounds
   const Item & operator [] (int index) const 
   {
      checkIndex ( index );
      return *( myZero + index );
   }

private:
   Item * myList;  // the array of items
   Item * myZero; // the pointer to the logical zero
   int myCapacity; // number of things in the vector
   int lowerb; // the lower bound
   int upperb; // the upper bound

   // aborts with appropriate message if index is not in appropriate range
   void checkIndex (int index) const 
   {
      if (( index < lowerb ) || ( index > upperb )) 
      {
         std::cerr << "Illegal vector index: " << index
                   << " (max = " << upperb << ")"
                   << " (min = " << lowerb << ")" 
                   << std::endl;
         assert(( index >= lowerb ) && ( index <= upperb ));
      }
   }
};

#endif                // _LBVECTOR_H not defined
