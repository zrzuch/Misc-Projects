import java.util.Scanner;
import java.util.Arrays;
import java.lang.Thread;

public class PrimeThread
{
	public static void main(String[] args)
	{
		Scanner input = new Scanner(System.in);
		System.out.print("Enter a number: ");
		int n = input.nextInt();
		Thread thread = new Thread(new Prime(n));
		thread.run();
	}

	private static class Prime implements Runnable
	{
		int n;

		public Prime(int n)
		{
			this.n = n;
		}

		public void run()
		{
			boolean[] isPrime;
			isPrime = new boolean[n - 1];
			Arrays.fill(isPrime, true);
			for (int i = 2; i < n; ++i)
			{
				if (isPrime[i - 2])
				{
					for (int j = 2 * i; j <= n; j += i)
					{
						isPrime[j - 2] = false;
					}
				}
			}
			for (int i = 0; i < isPrime.length; ++i)
			{
				if (isPrime[i])
				{
					System.out.println(i + 2);
				}
			}
		}
	}
}