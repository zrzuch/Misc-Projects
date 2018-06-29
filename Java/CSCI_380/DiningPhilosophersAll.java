public class Philosopher implements Runnable
{
	private DiningServer server;
	private int philNum;

	public Philosopher(DiningServer server, int philNum)
	{
		this.server 	= server;
		this.philNum 	= philNum;
	}

	private void thinking()
	{
		SleepUtilities.nap();
	}

	private void eating()
	{
		SleepUtilities.nap();
	}
		// philosophers alternate between thinking and eating
	public void run()
	{
		while(true)
		{
			System.out.println("philosopher " + philNum + " is thinking.");
			thinking();

			System.out.println("philosopher " + philNum + " is hungry.");
			server.takeForks(philNum);
			
			System.out.println("philosopher " + philNum + " is eating.");
			eating();
			
			System.out.println("philosopher " + philNum + " is done eating.");
			server.returnForks(philNum);
		}
	}
}

public interface DiningServer
{
	// called by a philosopher when they wish to eat
	public void takeForks(int philNumber);
	// called by a philosopher when they are finished eating
	public void returnForks(int philNumber);
}

public class DiningPhilosophers
{ 
	public static void main(String args[])
	{
		DiningServer server = new DiningServerImpl();
		Philosopher[] philosopherArray = new Philosopher[DiningServerImpl.NUM_OF_PHILS];
		// create the philosopher threads
		for (int i = 0; i < DiningServerImpl.NUM_OF_PHILS; ++i)
		{
			philosopherArray[i] = new Philosopher(server, i);
		}
		Thread[] threads = new Thread[DiningServerImpl.NUM_OF_PHILS];
		for (int i = 0; i < DiningServerImpl.NUM_OF_PHILS; ++i)
		{
			threads[i] = new Thread(philosopherArray[i]);
		}
		for (int i = 0; i < DiningServerImpl.NUM_OF_PHILS; ++i)
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

public class SleepUtilities
{
	static final int NAP_TIME = 5;

	/**
	* Nap between zero and NAP_TIME seconds.
	*/
	public static void nap()
	{
		// has to be a long and its in milliseconds
		Thread.sleep((long) Math.ceil(Math.random() * NAP_TIME * 1000));
	}
}

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class DiningServerImpl implements DiningServer 
{
	private static final int NUM_OF_PHILS = 5;
	private int[] philEating;

	private final Lock lock = new ReentrantLock();

	// private final Condition leftReady = lock.newCondition();
 //    private final Condition rightReady = lock.newCondition();
	private final Condition readyToEat = lock.newCondition();

	// different philosopher states
	enum State 
	{
		THINKING, 
		HUNGRY, 
		EATING
	};

	public DiningServerImpl()
	{
		int n = NUM_OF_PHILS / 2;
		philEating = new int[n];
		for (int i = 0; i < n; ++i)
		{
			philEating[i] = -1;
		}
	}

	public static int findGreater(int n, int max)
	{
		return ( n >= max - 1 ) ? ( 0 ) : ( n + 1 );
	}

	public static int findLesser(int n, int max)
	{
		return ( n <= 0 ) ? ( max ) : ( n - 1 );
	}

	public boolean isLeftEating(int philNumber)
	{
		int leftPhil = findLesser(philNumber, NUM_OF_PHILS);
		for (int i = 0; i < philEating.length; ++i)
		{
			if (philEating[i] == leftPhil)
			{
				return true;
			}
		}
		return false;
	}

	public boolean isRightEating(int philNumber)
	{
		int rightPhil = findGreater(philNumber, NUM_OF_PHILS);
		for (int i = 0; i < philEating.length; ++i)
		{
			if (philEating[i] == rightPhil)
			{
				return true;
			}
		}
		return false;
	}

	// called by a philosopher when they wish to eat
	public void takeForks(int philNumber)
	{
		lock.lock();

		try
		{
			while(isLeftEating(philNumber) || isRightEating(philNumber))
			{
				readyToEat.await();
			}
			for (int i = 0; i < philEating.length; ++i)
			{
				if (philEating[i] == -1)
				{
					philEating[i] = philNumber;
				}
			}
		}
		finally
		{
			lock.unlock();
		}
	}

	// called by a philosopher when they are finished eating
	public void returnForks(int philNumber)
	{
		lock.lock();

		try
		{
			for (int i = 0; i < philEating.length; ++i)
			{
				if (philEating[i] == philNumber)
				{
					philEating[i] = -1;
				}
			}
		}
		finally
		{
			lock.unlock();
		}
	}
}