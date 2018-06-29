import java.util.concurrent.ExecutorService;
import java.util.concurrent.TimeUnit;
import java.lang.InterruptedException;
import java.util.concurrent.Executors;
import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.Future;
import java.util.concurrent.Callable;

public class MatrixThreadPool
{
	public static void main(String[] args)
	{
		Matrix matrix = new Matrix(3, 2, 3,
		new int[][]
		{
			{1, 4},
			{2, 5},
			{3, 6}
		}, 
		new int[][]
		{
			{8, 7, 6},
			{5, 4, 3}
		});

		ExecutorService pool = Executors.newCachedThreadPool();

		List<Callable<Object>> tasks = new ArrayList<Callable<Object>>(1);

		tasks.add(Executors.callable(matrix));

		try
		{
			List<Future<Object>> answers = pool.invokeAll(tasks);
		}
		catch (InterruptedException ie)
		{
			System.err.println(ie);
		}

		matrix.printPoduct();

		pool.shutdown();
	}

	private static class Matrix implements Runnable
	{
		private int M;
		private int K;
		private int N;

		private int[][] A;
		private int[][] B;
		private int[][] C;

		public Matrix(int m, int k, int n, int[][] a, int[][] b)
		{
			M = m;
			K = k;
			N = n;
			A = a;
			B = b;
			C = new int[M][N];
		}

		public void run()
		{
			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					C[i][j] = dot(i, j);
				}
			}
		}

		public int dot(int row, int column)
		{
			int sum = 0;
			for (int i = 0; i < K; ++i)
			{
				sum += A[row][i] * B[i][column];
			}
			return sum;
		}

		public int[][] getProduct()
		{
			return C;
		}

		public void printPoduct()
		{
			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					System.out.print(C[i][j] + " ");
				}
				System.out.println();
			}
			System.out.println();
		}
	}
}