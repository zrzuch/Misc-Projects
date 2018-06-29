import java.net.Socket;
import java.io.InputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

public class Client
{
	public static void main(String[] args)
	{
		try
		{
			Socket socket = new Socket("127.0.0.1", 6013);
			InputStream in = socket.getInputStream();
			BufferedReader br = new BufferedReader(new InputStreamReader(in));

			String line;
			while ( (line = br.readLine()) != null )
			{
				System.out.println(line);
			}
			socket.close();
		}
		catch (IOException ioe)
		{
			System.err.println(ioe);
		}
	}
}