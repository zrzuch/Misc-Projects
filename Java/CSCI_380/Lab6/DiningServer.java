public interface DiningServer
{
	// called by a philosopher when they wish to eat
	public void takeForks(int philNumber);
	// called by a philosopher when they are finished eating
	public void returnForks(int philNumber);
}