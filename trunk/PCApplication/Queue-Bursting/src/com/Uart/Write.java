package com.Uart;

import java.io.*;
import java.util.*;
import javax.comm.*;

public class Write {
	static Enumeration portList;
	static CommPortIdentifier portId;
	static String messageString = "Hello, world!\n";
	static SerialPort serialPort;
	static OutputStream outputStream;

	public void WriteData(String NumCom, String Out) {
		// portList = CommPortIdentifier.getPortIdentifiers(NumCom);
		try {
			portId = CommPortIdentifier.getPortIdentifier(NumCom);
		} catch (NoSuchPortException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		// while (portList.hasMoreElements()) {
		// portId = (CommPortIdentifier) portList.nextElement();
		// if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
		// if (portId.getName().equals(NumCom)) {
		// // if (portId.getName().equals("/dev/term/a")) {
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
	// //// }
	// // }
	// }
}
