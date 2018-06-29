#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <ctime>

#include "Timer.h"

using std::cout;
using std::endl;
using std::sort;
using std::copy;
using std::vector;

int
main (int argc, char* argv[])
{
	srand(time(0));
	const size_t ARRAY_SIZE = 100;

	int* arr = new int[ ARRAY_SIZE ];
	for ( size_t i = 0; i < ARRAY_SIZE; ++i )
	{
		arr[ i ] = ( rand() % ARRAY_SIZE ) + 1;
	}

	Timer t;
	t.start( );
	sort( arr, arr + ARRAY_SIZE );
	t.stop( );

	cout << "Elapsed Time = " << t.getElapsedMs() << "ms";

	delete[] arr;
	return EXIT_SUCCESS;
}