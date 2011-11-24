package com.Uart;

import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.TooManyListenersException;

import javax.comm.CommPortIdentifier;
import javax.comm.PortInUseException;
import javax.comm.SerialPort;
import javax.comm.SerialPortEvent;
import javax.comm.SerialPortEventListener;
import javax.comm.UnsupportedCommOperationException;

import com.Gui.MainGui;

public class Read implements Runnable, SerialPortEventListener {
	static CommPortIdentifier portId;
	static Enumeration portList;
	public InputStream inputStream;
	public static SerialPort serialPort;
	public Thread readThread;
	int numBytes;
	public static byte[] readBuffer = new byte[10000];
	public static boolean bError = false;
	public static boolean bStop = false;

	/**
	 * Method declaration
	 * 
	 * 
	 * @param args
	 * 
	 * @see
	 */
//	public static void main(String arg[]) {
//		boolean portFound = false;
//		String defaultPort = "COM2";
//
//		portList = CommPortIdentifier.getPortIdentifiers();
//
//		while (portList.hasMoreElements()) {
//			portId = (CommPortIdentifier) portList.nextElement();
//			if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
//				if (portId.getName().equals(defaultPort)) {
//					System.out.println("Found port: " + defaultPort);
//					portFound = true;
//					Read reader = new Read(serialPort,portId);
//		
//				}
//			}
//		}
//		if (!portFound) {
//			System.out.println("port " + defaultPort + " not found.");
//		}
//
//	}

	/**
	 * Constructor declaration
	 * 
	 * 
	 * @see
	 */

	public Read(CommPortIdentifier portId)
	{
		bError = false;
		try {
			serialPort = (SerialPort) portId.open("RecvData", 2000);
		} catch (PortInUseException e) {
			System.out.println("something error");//portId.getName() + " is busy"+ " "+ portId.getCurrentOwner());
			bError = true; 
		}
		if(!bError){
			try {
				inputStream = serialPort.getInputStream();
			} catch (IOException e) {
				bError = true; 
			}
			if(!bError){
				try {
					serialPort.addEventListener(this);
				} catch (TooManyListenersException e) {
					bError = true; 
				}
				if(!bError){
					serialPort.notifyOnDataAvailable(true);
					try {
						serialPort.setSerialPortParams(9600, SerialPort.DATABITS_8,
								SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
					} catch (UnsupportedCommOperationException e) {
						bError = true; 
					}
					if(!bError){
						readThread = new Thread(this);
						readThread.start();
					}
				}
			}
		}
		
	}

	/**
	 * Method declaration
	 * 
	 * 
	 * @see
	 */
	public void run() {
			try {
				Thread.sleep(20000);
			} catch (InterruptedException e) {
				System.out.println("Exception run");
				
			}
	}

	/**
	 * Method declaration
	 * 
	 * 
	 * @param event
	 * 
	 * @see
	 */
	public void serialEvent(SerialPortEvent event) {
		switch (event.getEventType()) {

		case SerialPortEvent.BI:

		case SerialPortEvent.OE:

		case SerialPortEvent.FE:

		case SerialPortEvent.PE:

		case SerialPortEvent.CD:

		case SerialPortEvent.CTS:

		case SerialPortEvent.DSR:

		case SerialPortEvent.RI:

		case SerialPortEvent.OUTPUT_BUFFER_EMPTY:
			break;

		case SerialPortEvent.DATA_AVAILABLE:

			try {
				Thread.sleep(500);
				while (inputStream.available() > 0) {
					numBytes = inputStream.read(readBuffer);
					
				}
				String _sIdentify = new String(readBuffer).substring(0, 1);
				switch (_sIdentify.charAt(0)){
				case 'H':MainGui.ProcessData(0);
						break;
				case '+':
				case '-':MainGui.ProcessData(1);
						break;
				default:break;
				}
				
				
					
			} catch (IOException e) {
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			break;
		}
	}

}