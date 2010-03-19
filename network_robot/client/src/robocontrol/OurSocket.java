/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package robocontrol;

import java.net.*;
import java.io.*;
/**
 *
 *
 */
public class OurSocket {
    Socket socket;
    DataInputStream theReader;
    DataOutputStream theWriter;

public OurSocket (String host, int port)
        throws UnknownHostException, IOException
{
    this(new Socket(host, port));
}

public OurSocket (Socket s) throws UnknownHostException, IOException {
    socket = s;
    theReader =  new DataInputStream(socket.getInputStream());
    theWriter =  new DataOutputStream(socket.getOutputStream());
}

public Socket getSocket ()
{
    return socket;
}

public void close() throws IOException {
    socket.close();
}

public int readLine() throws IOException {
    return theReader.readInt();
}

public void println (int s) throws IOException {
    theWriter.writeInt(s);
}
}