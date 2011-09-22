import java.io.*;
import java.net.*;

public class NSServerThread extends Thread {

	Socket client;
	BufferedReader in;
	PrintWriter out;

	public NSServerThread(Socket accept) throws IOException {
		this.client = accept;
		System.out.println("Accepting connection");
		
		try {
			in = new BufferedReader(new InputStreamReader(client.getInputStream()));
			out = new PrintWriter(client.getOutputStream(), true);
		} catch (IOException e) {
			System.out.println("Could not read from client");
			try {
				client.close();
			} catch (IOException e1) {
				System.out.println("Could not close client connection");
			}
		}
		
		String input, ip = "";
		InetAddress host;
		while ((input = in.readLine()) != null) {
			if (input.toLowerCase().substring(0, 4).equals("exit"))
				break;
			
			System.out.printf("Looking up host: %s\n", input);
			
			// Look up ip address
			try {
				host = InetAddress.getByName(input.trim());
				ip = host.getHostAddress();
			} catch (UnknownHostException e) {
				ip = "Unable to resolve host " + input;
			}
			
			out.println(ip);
		}
		
		System.out.println("Closing connection");
		in.close();
		out.close();
		client.close();
	}
}