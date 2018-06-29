/*
  Filename   : Stack.hpp
  Author     : Zachary Zuch
  Course     : CSCI 362-02
  Assignment : Lab 3
  Description: This class is my implementation of a stack
  				using a vector.
*/

/************************************************************/
// System includes

#include <iostream>
#include <vector>

/************************************************************/
// Using declarations

using std::vector;
using std::ostream;
using std::cout;

/************************************************************/

template <typename T> 
class Stack
{
public:

	Stack ()
		: m_stack ( )
	{
	}

	void
	push(const T& e)
	{
		m_stack.push_back(e);
	}

	T&
	top ()
	{
		return m_stack[size()];
	}

	const T&
	top () const
	{
		return m_stack[size()];
	}

	void
	pop ()
	{
		m_stack.pop_back();
	}

	size_t
	size () const
	{
		return m_stack.size();
	}

	bool
	empty () const
	{
		return m_stack.empty();
	}

	void
	display (ostream& os) const
	{
		os << "[ ";
		for (size_t i = size(); i > 0; --i)
		{
			cout << m_stack[i - 1] << " ";
		}
		os << "]";
	}

private:

	vector<T> m_stack;
};

/************************************************************/

template <typename T>
ostream&
operator<<(ostream& os, Stack<T>& s)
{
	s.display(os);
	return os;
}

/************************************************************/