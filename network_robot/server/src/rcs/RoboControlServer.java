/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package robocontrol.server;

import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.util.*;
//import java.util.logging.Logger;
import javax.comm.*;
/**
 *
 *
 */
    public class RoboControlServer implements Runnable {

        public int port = 0;
        public ServerSocket sst;
        public OurSocket os;
        boolean run;
        public Thread t;
        public InetAddress clientAddr;
        public int clientPort;
        private boolean connected = false;
        private ServerApp serverApp;
        static SerialPort serialPort;
        static Enumeration portList;
        static CommPortIdentifier portId;
        static OutputStream outputStream;

        
        RoboControlServer(ServerApp s) {
            this.serverApp = s;
            //this.init_serialport();
            run = true;
            t = new Thread(this, "RoboControl Server");
            
            t.start();
        }
        
        public void stop()
        {
        try {
            this.connected = false;
            this.run = false;
            this.t.interrupt();
            if (os!=null)
                this.os.close();
            if (sst!=null)
                this.sst.close();
            os = null;
            sst = null;
            serverApp.UpdateRoboControlServerConnStatus(connected);
        } catch (IOException ex) {
            serverApp.showConsoleMessage(ex.getMessage());
        }
        }
        
        public boolean isConnected ()
        {
            return connected;
        }
        
        public void init_serialport()
        {
            boolean portFound = false;
            String  defaultPort = "/dev/ttyS0";

            portList = CommPortIdentifier.getPortIdentifiers();

            while (portList.hasMoreElements()) 
            {
                portId = (CommPortIdentifier) portList.nextElement();
                if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) 
                {
        		if (portId.getName().equals(defaultPort)) 
                        {
                             serverApp.showConsoleMessage("Found port " + defaultPort);
                            portFound = true;
                            try 
                            {
                                serialPort =  (SerialPort) portId.open("SimpleWrite", 2000);
                            }
                            catch (PortInUseException e) 
                            {
                                 serverApp.showConsoleMessage ("Port in use.");
                                continue;
                            } 

                            try 
                            {
                                outputStream = serialPort.getOutputStream();
                            } 
                            catch (IOException e) {}

                            try 
                            {
                                serialPort.setSerialPortParams(19200, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_EVEN);
                            }
                            catch (UnsupportedCommOperationException e) {}
	

                           try 
                           {
                                serialPort.notifyOnOutputEmpty(true);
                            } 
                           catch (Exception e) 
                           {
                                System.out.println("Error setting event notification");
                                System.out.println(e.toString());
                                System.exit(-1);
                            }
		                           
                            /*try 
                            {
                                outputStream.write(messageString.getBytes());
                            }
                            catch (IOException e) {}

                            try 
                            {
                                Thread.sleep(2000);  // Be sure data is xferred before closing
                            }
                            catch (Exception e) {}
                            serialPort.close();
		    */
		} 
	    } 
	 
                if (!portFound) 
                {
                    System.out.println("port " + defaultPort + " not found.");
                } 
            } 
        }
        
        public void run() 
        {
            run = true;
            try {
                port = serverApp.getRoboControlServerPort();
                sst = new ServerSocket(port);
                serverApp.showConsoleMessage("RoboControl Server is listening at port: " + port);
                while (run) {
                    serverApp.showConsoleMessage ("Listening for connection request..");
                    os = new OurSocket(sst.accept());
                    serverApp.showConsoleMessage ("Received connection request from " + os.socket.getInetAddress().getHostAddress() + ":" + os.socket.getPort());

                    int cmd=os.reader.readInt();
                    
                    if (cmd == OurSocket.ROBOCONTROL_CONNECT_REQ) {
                            connected = true;
                            clientAddr = os.socket.getInetAddress();
                            clientPort = os.socket.getPort();
                            serverApp.showConsoleMessage ("Connection Established with client "+ os.socket.getInetAddress().getHostAddress() + ":" + os.socket.getPort());
                            //os.writer.writeInt(OurSocket.ROBOCONTROL_ACK);
                            serverApp.UpdateRoboControlServerConnStatus(connected);
                            
                            while (connected && os.socket.isConnected())
                            {
                                cmd = os.reader.readInt();
                                switch (cmd)
                                {
                                    case OurSocket.ROBOCONTROL_DISCONNECT_REQ:
                                        serverApp.showConsoleMessage ("Server: Received Client Disconnection Request");
                                        connected = false;
                                        break;
                                    case OurSocket.ROBOCONTROL_LISTENER_SERVER_READY_REQ:
                                        serverApp.showConsoleMessage ("Server: Received Listener Ready Request");
                                        serverApp.connectToRoboControlListenerServer(os.reader.readInt());
                                        break;
                                    case OurSocket.ROBOCONTROL_PING_REQ:
                                        serverApp.showConsoleMessage ("Server: Received Ping Request");
                                        os.writer.writeInt(OurSocket.ROBOCONTROL_PING_REPLY);
                                        break;
                                    case OurSocket.CONTROL_PRESSED_UP:
                                        //int p = os.reader.readInt();
                                        //serverApp.listener.doPressControl(p);
                                        serverApp.showConsoleMessage ("Server: Control UP Pressed ");
                                        outputStream.write(00001100);
                                        break;
                                    case OurSocket.CONTROL_RELEASED_UP:
                                        //int c = os.reader.readInt();
                                        //serverApp.listener.doReleaseAll();
                                        serverApp.showConsoleMessage ("Server: Control UP Released ");
                                        break;
                                     case OurSocket.CONTROL_PRESSED_DOWN:
                                        //int p = os.reader.readInt();
                                        //serverApp.listener.doPressControl(p);
                                        serverApp.showConsoleMessage ("Server: Control DOWN Pressed " );
                                        outputStream.write(00001000);
                                        break;
                                    case OurSocket.CONTROL_RELEASED_DOWN:
                                        //int c = os.reader.readInt();
                                        //serverApp.listener.doReleaseAll();
                                        serverApp.showConsoleMessage ("Server: Control DOWN Released ");
                                        break;
                                    case OurSocket.CONTROL_PRESSED_LEFT:
                                        //int p = os.reader.readInt();
                                        //serverApp.listener.doPressControl(p);
                                        serverApp.showConsoleMessage ("Server: Control LEFT Pressed " );
                                        outputStream.write(00000000);
                                        break;
                                    case OurSocket.CONTROL_RELEASED_LEFT:
                                        //int c = os.reader.readInt();
                                        //serverApp.listener.doReleaseAll();
                                        serverApp.showConsoleMessage ("Server: Control LEFT Released ");
                                        break;
                                    case OurSocket.CONTROL_PRESSED_RIGHT:
                                        //int p = os.reader.readInt();
                                        //serverApp.listener.doPressControl(p);
                                        serverApp.showConsoleMessage ("Server: Control RIGHT Pressed " );
                                        outputStream.write(00000010);
                                        break;
                                    case OurSocket.CONTROL_RELEASED_RIGHT:
                                        //int c = os.reader.readInt();
                                        //serverApp.listener.doReleaseAll();
                                        serverApp.showConsoleMessage ("Server: Control  RIGHT Released ");
                                        break;
                                    case OurSocket.CONTROL_PRESSED_STOP:
                                        //int p = os.reader.readInt();
                                        //serverApp.listener.doPressControl(p);
                                        serverApp.showConsoleMessage ("Server: Control STOP Pressed " );
                                        break;
                                    case OurSocket.CONTROL_RELEASED_STOP:
                                        //int c = os.reader.readInt();
                                        //serverApp.listener.doReleaseAll();
                                        serverApp.showConsoleMessage ("Server: Control STOP Released " );
                                        break;
                                    
                                }
                                //os.writer.writeInt(OurSocket.ROBOCONTROL_ACK);
                            }
                            serverApp.showConsoleMessage ("Disconnected from client "+ os.socket.getInetAddress().getHostAddress() + ":" + os.socket.getPort());
                            serverApp.UpdateRoboControlServerConnStatus(connected);
                    } else {
                            serverApp.showConsoleMessage ("Received invalid connection request");
                        }
                    }
                    serverApp.showConsoleMessage ("Shutting down RoboControl Server..");
                    os.close();
                sst.close();
                connected = false;
                serverApp.UpdateRoboControlServerConnStatus(connected);
            } catch (Exception ex) {
                serverApp.showConsoleMessage("RoboControlServer Error: " + ex.getMessage());
            }
        }


    }