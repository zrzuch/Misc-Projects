import java.lang.Thread;
import java.lang.InterruptedException;

public class SleepUtilities
{
	static final int NAP_TIME = 5;

	/**
	* Nap between zero and NAP_TIME seconds.
	*/
	public static void nap()
	{
		try
		{
			// has to be a long and its in milliseconds
			Thread.sleep((long) Math.ceil(Math.random() * NAP_TIME * 1000));
		}
		catch(InterruptedException e)
		{
			System.err.println(e);
		}
	}
}