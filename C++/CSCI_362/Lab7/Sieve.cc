#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::iota;
using std::set;
using std::string;
using std::stoul;
using std::vector;

set<unsigned>
sieveSet (unsigned n);

set<unsigned>
sieveVector (unsigned n);

int
main ( int argc, char* argv[] )
{
	if ( argc != 3 )
	{
		cerr << "Usage: " << argv[0] << " <Type> <N>" << endl;
    	exit (EXIT_FAILURE);
	}

	string arg1 ( argv[ 1 ] );
	string arg2 ( argv[ 2 ] );
	unsigned N = stoul ( arg2 );
	unsigned numOfPrimes;

	if ( arg1 == "set" ) numOfPrimes = sieveSet ( N ).size();

	else if ( arg1 == "vector" ) numOfPrimes = sieveVector ( N ).size();

	else
	{
		cerr << "Usage: " << argv[0] << " <Type> <N>" << endl;
    	exit (EXIT_FAILURE);
	}

	cout << "Pi[" << N << "] = " << numOfPrimes << " (using a " 
		<< arg1 << ")" << endl;

	return EXIT_SUCCESS;
}

// Set version
// Find and return the set of prime numbers from 2 to "n"
// Do NOT print ANYTHING
set<unsigned>
sieveSet (unsigned n)
{

}

// Vector version
// Find and return the set of prime numbers from 2 to "n"
// Do NOT print ANYTHING
set<unsigned>
sieveVector (unsigned n)
{
	set<unsigned> setOfPrimes;
	vector<unsigned> vecOfNumbers ( n - 1 );
	vector<bool> vecIsPrime ( n - 1 , true );
	iota (vecOfNumbers.begin(), vecOfNumbers.end(), 2);

	for ( size_t i = 0; i <= n - 2; ++i )
	{
		if ( vecIsPrime[i] )
		{
			for ( size_t m = vecOfNumbers[i]; 
				 ( vecOfNumbers[i] * m ) <= n; ++m )
			{
				vecIsPrime[ ( vecOfNumbers[i] * m ) - 2 ] = false;
			}
		}
	}

	for ( size_t i = 0; i <= n - 2; ++i )
	{
		if ( vecIsPrime[i] )
		{
			setOfPrimes.insert( i + 2 );
		}
	}

	return setOfPrimes;
}
