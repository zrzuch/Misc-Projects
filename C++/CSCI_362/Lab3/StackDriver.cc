/*
  Filename   : StackDriver.cc
  Author     : Zachary Zuch
  Course     : CSCI 362-02
  Assignment : Lab 3
  Description: This program is the driver for the implemented
  				stack class.
*/

/************************************************************/
// System includes

#include <iostream>
#include <cstdlib>
#include <string>

/************************************************************/
// Local includes

#include "Stack.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;

/************************************************************/

int
main(int argc, char* argv[])
{
	cout << "Number => ";
	int num;
	while ( (cin >> num) && (num < 0) )
	{}

	cout << "Base   => ";
	size_t base;
	while ( (cin >> base) && (base < 2 || base > 16) )
	{}

	Stack<char> s;
	char c[] = "0123456789ABCDEF";
	int temp = 0;
	while (num != 0)
	{
		temp = num % base;
		s.push(c[temp]);
		num /= base;
	}

	cout << s << endl;

	return EXIT_SUCCESS;
}

/************************************************************/