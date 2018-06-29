import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;
import java.lang.InterruptedException;

public class DiningServerImpl implements DiningServer 
{
	public static final int NUM_OF_PHILS = 5;
	private Lock lock = new ReentrantLock();

	private State[] philEating = new State[NUM_OF_PHILS];
	private Condition[] readyToEat = new Condition[NUM_OF_PHILS];

	// different philosopher states
	enum State 
	{
		THINKING, 
		HUNGRY, 
		EATING
	};

	public DiningServerImpl()
	{
		for (int i = 0; i < NUM_OF_PHILS; ++i)
		{
			philEating[i] = State.THINKING;
			readyToEat[i] = lock.newCondition();
		}
	}

	public static int findGreater(int n, int max)
	{
		return ( n + 1 ) % max;
	}

	public static int findLesser(int n, int max)
	{
		return ( n <= 0 ) ? ( max - 1 ) : ( n - 1 );
	}

	public boolean isLeftEating(int philNumber)
	{
		return (philEating[findLesser(philNumber, NUM_OF_PHILS)] == State.EATING) ? true : false;
	}

	public boolean isRightEating(int philNumber)
	{
		return (philEating[findGreater(philNumber, NUM_OF_PHILS)] == State.EATING) ? true : false;
	}

	// called by a philosopher when they wish to eat
	public void takeForks(int philNumber)
	{
		lock.lock();

		try
		{
			while (isLeftEating(philNumber) || isRightEating(philNumber))
			{
				try
				{
					philEating[philNumber] = State.HUNGRY;
					readyToEat[philNumber].await();
				}
				catch(InterruptedException e)
				{
					System.err.println(e);
				}
			}
			philEating[philNumber] = State.EATING;
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
			philEating[philNumber] = State.THINKING;
			readyToEat[findLesser (philNumber, NUM_OF_PHILS)].signal();
			readyToEat[findGreater(philNumber, NUM_OF_PHILS)].signal();
		}
		finally
		{
			lock.unlock();
		}
	}
}