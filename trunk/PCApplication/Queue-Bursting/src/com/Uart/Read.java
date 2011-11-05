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
	static InputStream inputStream;
	static SerialPort serialPort;
	public static Thread readThread;
	int numBytes;
	public static byte[] readBuffer = new byte[100];

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
//	public Read(){
//		
//	}
	public Read(CommPortIdentifier portId)
	{
		try {
			serialPort = (SerialPort) portId.open("RecvData", 2000);
		} catch (PortInUseException e) {
		}

		try {
			inputStream = serialPort.getInputStream();
		} catch (IOException e) {
		}

		try {
			serialPort.addEventListener(this);
		} catch (TooManyListenersException e) {
		}

		serialPort.notifyOnDataAvailable(true);

		try {
			serialPort.setSerialPortParams(9600, SerialPort.DATABITS_8,
					SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
		} catch (UnsupportedCommOperationException e) {
		}

		readThread = new Thread(this);

		readThread.start();
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
				while (inputStream.available() > 0) {
					numBytes = inputStream.read(readBuffer);
					
				}
				MainGui.ShowResult();
			} catch (IOException e) {
			}

			break;
		}
	}

}