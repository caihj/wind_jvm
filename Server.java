
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

    public static void main(String[] args) throws IOException {


        ServerSocket socket = new ServerSocket(8000);

        while (true) {
            Socket client = socket.accept();
            System.out.println("connect from :"+ client.getInetAddress().toString());
            client.getOutputStream().write("hello world\n".getBytes());
            client.getOutputStream().flush();
            client.close();
        }

    }
}
