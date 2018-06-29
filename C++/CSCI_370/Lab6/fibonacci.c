#include <stdio.h>
int fib (int n);
int zeros (int n);
int main(int argc, char* argv[])
{
	printf("n ==> ");
	int x;
	scanf("%d", &x);
	int f = fib(x);
	int z = zeros(x);
	printf("fib(|%d|) - zeros(%d) = %d - %d = %d\n", x , x , f , z , (f - z) );
}
int fib (int n)
{
	if (n < 0)
	{
		n *= -1;
	}
	if (n <= 1)
	{
		return 1;
	}
	int temp = 0;
	int temp2;
	int i = 1;
	while (i <= n)
	{
		temp2 = i;
		i += temp;
		temp = temp2;
	}
	return i;
}
int zeros (int n)
{
	int z = 0;
	int i;
	for(i = 0; i < 32; i++)
	{
		if ((n & (1 << i)) == 0)
		{
			z++;
		}
	}
	return z;
}
