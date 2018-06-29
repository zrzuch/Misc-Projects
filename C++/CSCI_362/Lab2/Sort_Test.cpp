#include <iostream>
#include <cstdlib>

#include <stdlib.h>
#include <time.h>

#include "Array.hpp"

using std::cout;
using std::endl;

void
randomizeArray(Array<int> &arr);

int 
main(int argc, char* argv[])
{
	srand(time(NULL));
	
	Array<int> A (10);
	randomizeArray(A);

	Array<int> B (10);
	randomizeArray(B);

	Array<int> C (10);
	randomizeArray(C);

	cout << A << endl;
	cout << "Array Sorted with Bubble Sort" << endl;
	A.bubbleSort();
	cout << A << endl << endl;

	cout << B << endl;
	cout << "Array Sorted with Selection Sort" << endl;
	B.selectionSort();
	cout << B << endl << endl;

	cout << C << endl;
	cout << "Array Sorted with Insertion Sort" << endl;
	C.insertionSort();
	cout << C << endl << endl;

	return EXIT_SUCCESS;
}

void
randomizeArray(Array<int> &arr)
{
	for (size_t i = 0; i < arr.size(); ++i)
	{
		arr[i] = rand() % 100 + 1;
	}
}