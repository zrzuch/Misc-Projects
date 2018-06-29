import java.util.Scanner;
import java.lang.Thread;
import java.util.Vector;
import java.lang.InterruptedException;

public class FibonacciThread
{
	public static void main(String[] args)
	{
		Scanner input = new Scanner(System.in);
		System.out.print("Enter a number: ");
		int n = input.nextInt();
		Fibonacci f = new Fibonacci(n);
		Thread thread = new Thread(f);
		thread.run();
		try
		{
			thread.join();
		}
		catch(InterruptedException e)
		{
			System.err.println(e);
		}
		f.print();
	}

	private static class Fibonacci implements Runnable
	{
		int n;
		Vector<Integer> numbers;

		public Fibonacci(int n)
		{
			this.n = n;
			numbers = new Vector<Integer>();
		}

		public void run()
		{
			int tmp;
			int n1 = 0;
			int n2 = 1;
			while ( n1 <= n )
			{
				numbers.add(n1);
				tmp = n1;
				n1 += n2;
				n2 = tmp;
			}
		}

		public void print()
		{
			for (int i = 0; i < numbers.size(); ++i)
			{
				System.out.println(numbers.elementAt(i));
			}
		}
	}
}