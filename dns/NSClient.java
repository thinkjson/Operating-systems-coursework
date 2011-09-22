import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class NSClient {
	public static void main(String[] args) throws IOException {
		Socket socket = new Socket("127.0.0.1", 6052);
		PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
		BufferedReader in = 
			new BufferedReader(new InputStreamReader(socket.getInputStream()));
	}
}