
import java.io.IOException;
import java.io.InputStream;
import java.io.StringReader;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

public class Client {
    public static void main(String[] args) throws IOException {

        Socket socket = new Socket();

        socket.connect(new InetSocketAddress("127.0.0.1",8000));
        InputStream in = socket.getInputStream();

        while (true){
            int ret = in.read();
            if(ret<0){
                break;
            }
            System.out.write(ret);
        }
    }
}
