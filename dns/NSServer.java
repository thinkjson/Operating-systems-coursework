import java.io.*;
import java.net.*;

public class NSServer {
	static final int port = 6052;
	
	public static void main(String[] args) throws IOException {
		ServerSocket socket = new ServerSocket(NSServer.port);
		boolean listening = true;

		System.out.println("Listening on port " + NSServer.port);
		Runtime.getRuntime().addShutdownHook(new ShutdownIntercept(socket));
		while (listening) {
			new NSServerThread(socket.accept()).start();
		}
	}
	
}