/*
  Filename   : ArrayDriver.cc
  Author     : Zachary Zuch
  Course     : CSCI 362-02
  Assignment : Lab2
  Description: Test some methods of the Array class
*/   

/************************************************************/
// System includes

#include <cstdlib>
#include <iostream>
#include <string>
#include <iterator>

/************************************************************/
// Local includes

#include "Array.hpp"

/************************************************************/
// Using declarations

using std::cin;
using std::cout;
using std::endl;
using std::ostream_iterator;

/************************************************************/
// Function prototypes/global vars/typedefs

void
print (Array<int> A);

/************************************************************/

int      
main (int argc, char* argv[]) 
{        
  size_t size;
  cout << "Please input size of array ==> ";
  cin >> size;

  Array<int> A (size);
  A.push_back (5);
  A.push_back (10);
  A.push_back (15);
  cout << size << " zeroes + 5 10 15\n";
  print (A);

  A.pop_back ();
  A.pop_back ();
  cout << size << " zeroes + 5\n";
  print (A);

  for (int i = 0; i < 10; ++i)
  {
    A.insert (A.begin (), i);
  }
  cout << "Inserted 9 8 ... 0 at beginning\n";
  print (A);

  for (Array<int>::iterator i = A.begin (); i != A.end (); )
  {
    i = A.erase (i);
  }
  cout << "Erased all elements of A\n";
  print (A);

  Array<int> B (3, 4);
  cout << "B (3, 4): 3 4's\n";
  print (B);

  Array<int> C (B.begin (), B.begin () + 2);
  cout << "C (B.begin (), B.begin () + 2): 4 4\n";
  print (C);
  
  B = A;
  cout << "B = A: empty\n";
  print (B);

  cout << "Inserting 0, 1, 2, 3, 4 at beginning of B\n";
  for (int i = 0; i < 5; ++i)
    B.insert (B.begin (), i);
  print (B);

  cout << "Printing B using the output operator\n";
  cout << B << endl;
  cout << endl;

  Array<int> D;
  cout << "Array D Before and After Resize From Big to Small" << endl;
  D.resize(5);
  print(D);
  D.resize(4);
  print(D);

  Array<int> E(D.begin(), D.end());
  E.reserve(E.size() * 2);
  cout << "Same as The Previous Array But With Twice the Size as Capacity" << endl;
  print(E);

  E.reserve(E.size());
  cout << "Same as The Previous Array" << endl;
  print(E);

  D.resize(0);
  E.resize(0);

  print(D);
  print(E);
  
  return EXIT_SUCCESS;
}

/************************************************************/
// Copy ctor will get called as A is passed by value
void
print (Array<int> A) 
{
  cout << "[ ";
  // Using a for-each loop, which employs iterators
  for (const auto& e : A)
  {
    cout << e << " ";
  }
  cout << "]" << endl;
  cout << "Size = " << A.size () << ", Capacity = " 
       << A.capacity () << endl << endl;
}

/************************************************************/
