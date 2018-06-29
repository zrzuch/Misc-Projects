import java.net.Socket;
import java.net.ServerSocket;
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
				String quoteLine1 = "The meaning of life is just to be alive.";
				String quoteLine2 = "It is so plain and so obvious and so simple.";
				String quoteLine3 = "And yet, everybody rushes around in a great";
				String quoteLine4 = "panic as if it were necessary to achieve";
				String quoteLine5 = "something beyond themselves.";
				String quoteLine6 = "    - Alan Watts";
				pout.println(quoteLine1);
				pout.println(quoteLine2);
				pout.println(quoteLine3);
				pout.println(quoteLine4);
				pout.println(quoteLine5);
				pout.println(quoteLine6);
				client.close();
			}
		}
		catch (IOException ioe)
		{
			System.err.println(ioe);
		}
	}
}