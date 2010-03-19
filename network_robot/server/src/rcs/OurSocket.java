/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package robocontrol.server;

import java.net.*;
import java.io.*;


public class OurSocket {
    Socket socket;
    public DataInputStream reader;
    public DataOutputStream writer;
    
    /** Client request to connect to server */
    public static final int ROBOCONTROL_CONNECT_REQ = 1;
    /** Client request to disconnect to server */
    public static final int ROBOCONTROL_DISCONNECT_REQ = 3;
    /** Client ping request */
    public static final int ROBOCONTROL_PING_REQ = 4;
    /* Server response for ping request */
    public static final int ROBOCONTROL_PING_REPLY = 5;
    /** Server response for acknowledgement */
    public static final int ROBOCONTROL_ACK = 2;
    /** Client request to tell server that Listener server is ready */
    public static final int ROBOCONTROL_LISTENER_SERVER_READY_REQ = 11;
    
    
    /** Client request when a control is pressed */
    public static final int CONTROL_PRESSED_UP = 32;
    /** Client request when a control is released */
    public static final int CONTROL_RELEASED_UP = 33;
    
    /** Client request when a control is pressed */
    public static final int CONTROL_PRESSED_DOWN = 34;
    /** Client request when a control is released */
    public static final int CONTROL_RELEASED_DOWN = 35;
    
    /** Client request when a control is pressed */
    public static final int CONTROL_PRESSED_LEFT = 36;
    /** Client request when a control is released */
    public static final int CONTROL_RELEASED_LEFT = 37;
    
    /** Client request when a control is pressed */
    public static final int CONTROL_PRESSED_RIGHT = 38;
    /** Client request when a control is released */
    public static final int CONTROL_RELEASED_RIGHT = 39;
    
    /** Client request when a control is pressed */
    public static final int CONTROL_PRESSED_STOP = 40;
    /** Client request when a control is released */
    public static final int CONTROL_RELEASED_STOP = 41;
    
public OurSocket (String host, int port)
        throws UnknownHostException, IOException
{
    this(new Socket(host, port));
}

public OurSocket (Socket s) throws UnknownHostException, IOException {
    socket = s;
    reader = new DataInputStream(socket.getInputStream());
    writer = new DataOutputStream(socket.getOutputStream());
}

public Socket getSocket ()
{
    return socket;
}

public void close() throws IOException {
    reader.close();
    writer.close();
    socket.close();
}
}