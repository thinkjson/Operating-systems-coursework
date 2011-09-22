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
		BufferedReader query = 
			new BufferedReader(new InputStreamReader(System.in));
		
		String queryString, response;
		System.out.println("Look up hosts, one per line, or type `exit` to quit.");
		System.out.print("> ");
		while ((queryString = query.readLine()) != null) {
			
			// Look up ip address
			out.println(queryString);
			response = in.readLine();
			
			// Watch for DNS server disconnect
			if (response == null) {
				break;
			}
			
			System.out.println(response);
			System.out.print("> ");
		}
		
		in.close();
		out.close();
		socket.close();
	}
}