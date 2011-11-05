package com.Uart;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Enumeration;

import javax.comm.CommPortIdentifier;
import javax.comm.PortInUseException;
import javax.comm.SerialPort;
import javax.comm.UnsupportedCommOperationException;

public class Write {
	static Enumeration portList;
	static CommPortIdentifier portId;
	static String messageString = "Hello, world!\n";
	static SerialPort serialPort;
	static OutputStream outputStream;
	public Write(){
		
	}

	public void WriteData(String NumCom, String Out) {
		 portList = CommPortIdentifier.getPortIdentifiers();
		 while (portList.hasMoreElements()) {
		 portId = (CommPortIdentifier) portList.nextElement();
		 if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
		 if (portId.getName().equals(NumCom)) {
		try {
			serialPort = (SerialPort) portId.open("SendData", 2000);
		} catch (PortInUseException e) {
		}

		try {
			outputStream = serialPort.getOutputStream();
		} catch (IOException e) {
		}

		try {
			serialPort.setSerialPortParams(9600, SerialPort.DATABITS_8,
					SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
		} catch (UnsupportedCommOperationException e) {
		}

		try {
			outputStream.write(Out.getBytes());
		} catch (IOException e) {
		}

	}
	  }
	  }
	 }
	public static void main(String args[]) {
		Write test = new Write();
		test.WriteData("COM1", "hoang phuc");
		System.out.println("done!");
		
	}
}
