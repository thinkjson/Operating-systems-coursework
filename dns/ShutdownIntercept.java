import java.io.IOException;
import java.net.ServerSocket;

public class ShutdownIntercept extends Thread {
	ServerSocket socket;
	
	public ShutdownIntercept(ServerSocket socket) {
		this.socket = socket;
	}

	public void run() {
		System.out.println("Closing socket...");
		try {
			this.socket.close();
		} catch (IOException e) {
			System.out.println("Could not close socket.");
		}
	}
}