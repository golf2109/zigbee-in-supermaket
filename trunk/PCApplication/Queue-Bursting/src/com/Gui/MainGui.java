package com.Gui;

import java.awt.Rectangle;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.JScrollPane;
import javax.swing.JCheckBox;

public class MainGui extends JFrame {

	private static final long serialVersionUID = 1L;
	JPanel jContentPane = null;
	JButton jButtonConnect = null;
	JLabel jLabelPort = null;
	JTextField jTextFieldPort = null;
	JTextPane jTextPane = null;
	JButton jButtonDisconnect = null;
	JButton jButtonExit = null;
	JButton jButtonClear = null;
	JLabel jLabel = null;
	Thread thread1;
	Socket skt;
	
	private JButton jButtonTest = null;
	private JScrollPane jScrollPane = null;
	private JLabel jLabel1IP = null;
	private JTextField jTextFieldIP = null;
	String HostIP;
	int port;
	private JCheckBox jCheckBoxUART = null;
	private JCheckBox jCheckBoxETHERNET = null;
	/**
	 * This is the default constructor
	 */
	public MainGui() {
		super();
		initialize();
	}

	
	
	class RunnableThread implements Runnable {

		Thread runner;
		public RunnableThread() {
		}
		public RunnableThread(String threadName) {
			runner = new Thread(this, threadName); // (1) Create a new thread.
			System.out.println(runner.getName());
			runner.start(); // (2) Start the thread.
		}
		public void run() {
			//Display info about this particular thread
			System.out.println(Thread.currentThread());
		}
	}
	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	public void initialize() {
		this.setSize(521, 467);
		this.setContentPane(getJContentPane());
		this.setTitle("Casher");
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJContentPane() {
		if (jContentPane == null) {
			jLabel1IP = new JLabel();
			jLabel1IP.setBounds(new Rectangle(277, 342, 15, 38));
			jLabel1IP.setText("IP:");
			jLabel = new JLabel();
			jLabel.setBounds(new Rectangle(4, 36, 462, 23));
			jLabel
					.setText("Stt        Ma Hang                                    Ten Hang                                          Gia Ca");
			jLabelPort = new JLabel();
			jLabelPort.setBounds(new Rectangle(313, 380, 33, 38));
			jLabelPort.setText("Port:");
			jContentPane = new JPanel();
			jContentPane.setLayout(null);
			jContentPane.add(getJButtonConnect(), null);
			jContentPane.add(jLabelPort, null);
			jContentPane.add(getJTextFieldPort(), null);
			jContentPane.add(getJButtonExit(), null);
			jContentPane.add(getJButtonClear(), null);
			jContentPane.add(jLabel, null);
			jContentPane.add(getJButtonTest(), null);
			jContentPane.add(getJScrollPane(), null);
			jContentPane.add(getJButtonDisconnect(), null);
			jContentPane.add(jLabel1IP, null);
			jContentPane.add(getJTextFieldIP(), null);
			jContentPane.add(getJCheckBoxUART(), null);
			jContentPane.add(getJCheckBoxETHERNET(), null);
			setResizable(false);
		}
		return jContentPane;
	}

	/**
	 * This method initializes jButtonConnect
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJButtonConnect() {
		if (jButtonConnect == null) {
			jButtonConnect = new JButton();
			jButtonConnect.setBounds(new Rectangle(393, 344, 99, 35));
			jButtonConnect.setText("Connect");
			jButtonConnect.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("actionPerformed()"); // TODO Auto-generated Event stub actionPerformed()
//					thread1 = new Thread()
//					{
//					 @Override
//					 public void run()
//					 {
//						 connect();
//					 }
//					};
//					thread1.start();
//					jButtonConnect.setEnabled(false);
//				
//					jTextFieldPort.setEnabled(false);
				}
			});
			

		}
		return jButtonConnect;
	}

	/**
	 * This method initializes jTextFieldPort
	 * 
	 * @return javax.swing.JTextField
	 */
	private JTextField getJTextFieldPort() {
		if (jTextFieldPort == null) {
			jTextFieldPort = new JTextField();
			jTextFieldPort.setBounds(new Rectangle(355, 380, 34, 39));
			jTextFieldPort.setText("123");
			jTextFieldPort.addFocusListener(new java.awt.event.FocusListener() {
				public void focusLost(java.awt.event.FocusEvent e) {
					port = Integer.parseInt(jTextFieldPort.getText());
				}
				public void focusGained(java.awt.event.FocusEvent e) {
				}
			});
		}
		return jTextFieldPort;
	}

	/**
	 * This method initializes jTextPane
	 * 
	 * @return javax.swing.JTextPane
	 */
	private JTextPane getJTextPane() {
		if (jTextPane == null) {
			jTextPane = new JTextPane();
			jTextPane
					.setText("1           001020A                                       Product A                                          30000");
		}
		return jTextPane;
	}

	/**
	 * This method initializes jButtonDisconnect
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJButtonDisconnect() {
		if (jButtonDisconnect == null) {
			jButtonDisconnect = new JButton();
			jButtonDisconnect.setText("Disconnect");
			jButtonDisconnect.setBounds(new Rectangle(393, 382, 99, 34));
			jButtonDisconnect.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					jButtonConnect.setEnabled(true);
				
					jTextFieldPort.setEnabled(true);
					
					
				}
			});
		}
		return jButtonDisconnect;
	}

	/**
	 * This method initializes jButtonExit
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJButtonExit() {
		if (jButtonExit == null) {
			jButtonExit = new JButton();
			jButtonExit.setBounds(new Rectangle(50, 348, 86, 41));
			jButtonExit.setText("Exit");
			jButtonExit.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.exit(0);
				}
			});
		}
		return jButtonExit;
	}

	/**
	 * This method initializes jButtonClear
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJButtonClear() {
		if (jButtonClear == null) {
			jButtonClear = new JButton();
			jButtonClear.setBounds(new Rectangle(49, 296, 87, 41));
			jButtonClear.setText("Clear");
			jButtonClear.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
			
				}
			});
		}
		return jButtonClear;
	}

	public void connect() {
				String data = "Toobie ornaught toobie";
				while (true) {
					try { // Poll every ~10 ms
						Thread.sleep(10);
					} catch (InterruptedException e1) {
					}
					try {
						int port =Integer.parseInt(jTextFieldPort.getText());
						ServerSocket srvr = new ServerSocket(port);
						System.out.println("star listen");
						
						skt = srvr.accept();
						System.out.print("Server has connected!\n");
						PrintWriter out = new PrintWriter(
								skt.getOutputStream(), true);
						System.out.print("Sending string: '" + data + "'\n");
						out.print(data);
						out.close();
						skt.close();
						srvr.close();
					} catch (Exception err) {
						System.out.print("Whoops! It didn't work!\n");
					}
				
			
				}
	}
	public void connect1() {
		try {
	         Socket skt = new Socket("localhost", 123);
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

	/**
	 * This method initializes jButtonTest	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButtonTest() {
		if (jButtonTest == null) {
			jButtonTest = new JButton();
			jButtonTest.setBounds(new Rectangle(4, 356, 31, 57));
			jButtonTest.setText("Test");
			jButtonTest.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					connect1();
				}
			});
		}
		return jButtonTest;
	}

	/**
	 * This method initializes jScrollPane	
	 * 	
	 * @return javax.swing.JScrollPane	
	 */
	private JScrollPane getJScrollPane() {
		if (jScrollPane == null) {
			jScrollPane = new JScrollPane(jTextPane,
			         JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
			         JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
			jScrollPane.setBounds(new Rectangle(4, 60, 482, 213));
			jScrollPane.setViewportView(getJTextPane());
			
		}
		return jScrollPane;
	}

	/**
	 * This method initializes jTextFieldIP	
	 * 	
	 * @return javax.swing.JTextField	
	 */
	private JTextField getJTextFieldIP() {
		if (jTextFieldIP == null) {
			jTextFieldIP = new JTextField();
			jTextFieldIP.setBounds(new Rectangle(298, 343, 92, 37));
			jTextFieldIP.setText("192.168.13.12");
			jTextFieldIP.addFocusListener(new java.awt.event.FocusListener() {
				public void focusLost(java.awt.event.FocusEvent e) {
					jTextFieldIP.selectAll();
		            HostIP = jTextFieldIP.getText();
				}
				public void focusGained(java.awt.event.FocusEvent e) {
				}
			});
		}
		return jTextFieldIP;
	}

	/**
	 * This method initializes jCheckBoxUART	
	 * 	
	 * @return javax.swing.JCheckBox	
	 */
	private JCheckBox getJCheckBoxUART() {
		if (jCheckBoxUART == null) {
			jCheckBoxUART = new JCheckBox();
			jCheckBoxUART.setBounds(new Rectangle(311, 310, 69, 20));
			jCheckBoxUART.setText("UART");
		}
		return jCheckBoxUART;
	}

	/**
	 * This method initializes jCheckBoxETHERNET	
	 * 	
	 * @return javax.swing.JCheckBox	
	 */
	private JCheckBox getJCheckBoxETHERNET() {
		if (jCheckBoxETHERNET == null) {
			jCheckBoxETHERNET = new JCheckBox();
			jCheckBoxETHERNET.setBounds(new Rectangle(392, 313, 90, 17));
			jCheckBoxETHERNET.setText("ETHERNET");
		}
		return jCheckBoxETHERNET;
	}

//	public static void main(String[] args) {
//		//Start the threads
//	
//		//System.out.println(Thread.currentThread());
//	}

} // @jve:decl-index=0:visual-constraint="128,2"
