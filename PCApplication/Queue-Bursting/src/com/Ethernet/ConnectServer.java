package com.Ethernet;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.Socket;

public class ConnectServer {
	public void connect(String IP, int Port) {
		try {
	         Socket skt = new Socket(IP, Port);
	         BufferedReader in = new BufferedReader(new
	            InputStreamReader(skt.getInputStream()));
	         System.out.print("Received string: '");

	         while (!in.ready()) {}
	         System.out.println(in.readLine()); // Read one line and output it

	         System.out.print("'\n");
	         in.close();
	      }
	      catch(Exception e) {
	         System.out.print("Whoops! It didn't work!\n");
	      }
}

}