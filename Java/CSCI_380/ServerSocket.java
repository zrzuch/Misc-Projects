import java.net.Socket;
import java.io.PrintWriter;
import java.io.IOException;
import java.util.Date;

public class Server
{
	public static void main(String[] args)
	{
		try
		{
			ServerSocket socket = new ServerSocket(6013);
			while(true)
			{
				Socket client = socket.accept();
				PrintWriter pout = new 
					PrintWriter(client.getOutputStream(), true);
				pout.println(new Date().toString());
				client.close();
			}
		}
		catch (IOException ioe)
		{
			System.err.println(ioe);
		}
	}
}