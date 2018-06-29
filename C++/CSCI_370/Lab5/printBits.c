#include <stdio.h>

void
printBits(void* valuePtr, size_t size);

int
main(int argc, char* argv[])
{
	float x = -2.6;
	printBits ( &x, 4 ) ;
}

void
printBits(void* valuePtr, size_t size)
{
	unsigned char byte;
	int i;
	int j;

	for ( i = size - 1; i >= 0; i-- )
	{
		byte = *( ( ( unsigned char* ) valuePtr) + i );

		for ( j = 0; j < 8; j++ )
		{
			if ( ( byte & ( 128 >> j ) ) != 0 )
			{
				printf ( "1" );
			}
			else
			{
				printf ( "0" );
			}
		}
		printf ( " " );
	}
}
