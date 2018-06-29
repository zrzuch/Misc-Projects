import java.lang.Thread;
import java.lang.InterruptedException;

public class DiningPhilosophers
{ 
	public static void main(String args[])
	{
		DiningServer server = new DiningServerImpl();
		int max = DiningServerImpl.NUM_OF_PHILS;
		Philosopher[] philosopherArray = new Philosopher[max];
		// create the philosopher threads
		for (int i = 0; i < max; ++i)
		{
			philosopherArray[i] = new Philosopher(server, i);
		}
		Thread[] threads = new Thread[max];
		for (int i = 0; i < max; ++i)
		{
			threads[i] = new Thread(philosopherArray[i]);
		}
		for (int i = 0; i < max; ++i)
		{
			threads[i].start();
		}
		for (int i = 0; i < max; ++i)
		{
			try
			{
				threads[i].join();
			}
			catch(InterruptedException e)
			{
				System.err.println(e);
			}
		}
	}
}