package com.Gui;

import java.awt.Choice;
import java.awt.Rectangle;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;

import jxl.write.WriteException;

import com.RevData;
import com.Excel.WriteFile;

public class MainGui2 extends JFrame {

	private static final long serialVersionUID = 1L;
	JPanel jContentPane = null;  //  @jve:decl-index=0:visual-constraint="145,69"
	JButton jButtonConnect = null;
	JLabel jLabelPort = null;
	JTextField jTextFieldPort = null;
	JButton jButtonDisconnect = null;
	JButton jButtonExit = null;
	JButton jButtonClear = null;
	JLabel jLabel = null;
	Thread thread1;
	Socket skt;
	
	private JScrollPane jScrollPane = null;
	private JLabel jLabel1IP = null;
	private JTextField jTextFieldIP = null;
	String HostIP;
	int port;
	static String PathDatabase ="c:/temp/Database.xls";
	private JCheckBox jCheckBoxUART = null;
	private JCheckBox jCheckBoxETHERNET = null;
	private JButton jButtonOK = null;
	private Choice choice = null;
	private JList jList = null;
	private JLabel jLabel1PacketID = null;
	private JLabel jLabel1PID = null;
	/**
	 * This is the default constructor
	 */
	public MainGui2() {
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
		this.setTitle("Casher");
		this.setVisible(true);
		this.setContentPane(getJContentPane());
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJContentPane() {
		if (jContentPane == null) {
			jLabel1PID = new JLabel();
			jLabel1PID.setBounds(new Rectangle(333, 278, 47, 27));
			jLabel1PID.setText("");
			jLabel1PacketID = new JLabel();
			jLabel1PacketID.setBounds(new Rectangle(265, 276, 57, 32));
			jLabel1PacketID.setText("Packet Id:");
			jLabel1IP = new JLabel();
			jLabel1IP.setBounds(new Rectangle(277, 342, 15, 38));
			jLabel1IP.setText("IP:");
			
			jLabel1IP.setVisible(false);
			jLabel = new JLabel();
			jLabel.setBounds(new Rectangle(4, 36, 462, 23));
			jLabel
					.setText("Stt        Ma Hang                                    Ten Hang                                          Gia Ca");
			jLabelPort = new JLabel();
			jLabelPort.setBounds(new Rectangle(313, 380, 33, 38));
			jLabelPort.setText("Port:");
			jLabelPort.setVisible(false);
			jContentPane = new JPanel();
			jContentPane.setLayout(null);
			jContentPane.add(getJButtonConnect(), null);
			jContentPane.add(jLabelPort, null);
			jContentPane.add(getJTextFieldPort(), null);
			jContentPane.add(getJButtonExit(), null);
			jContentPane.add(getJButtonClear(), null);
			jContentPane.add(jLabel, null);
			jContentPane.add(getJScrollPane(), null);
			jContentPane.add(getJButtonDisconnect(), null);
			jContentPane.add(jLabel1IP, null);
			jContentPane.add(getJTextFieldIP(), null);
			jContentPane.add(getJCheckBoxUART(), null);
			jContentPane.add(getJCheckBoxETHERNET(), null);
			jContentPane.add(getJButtonOK(), null);
			jContentPane.add(getChoice(), null);
			jContentPane.add(jLabel1PacketID, null);
			jContentPane.add(jLabel1PID, null);
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
			jTextFieldPort.setVisible(false);
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
			jButtonExit.setBounds(new Rectangle(38, 375, 81, 41));
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
			jButtonClear.setBounds(new Rectangle(38, 333, 81, 41));
			jButtonClear.setText("Clear");
			jButtonClear.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					String []data={" "};
			         jList.setListData(data);
			         jLabel1PID.setText(null);
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
	 * This method initializes jScrollPane	
	 * 	
	 * @return javax.swing.JScrollPane	
	 */
	private JScrollPane getJScrollPane() {
		if (jScrollPane == null) {
			jScrollPane = new JScrollPane(jList,
			         JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
			         JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
			jScrollPane.setBounds(new Rectangle(4, 60, 482, 213));
			jScrollPane.setViewportView(getJList());
			
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
			jTextFieldIP.setVisible(false);
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
			jCheckBoxUART.setSelected(true);
			jCheckBoxUART.addMouseListener(new java.awt.event.MouseListener() {
				public void mouseClicked(java.awt.event.MouseEvent e) {
					choice.setVisible(true);
					jLabel1IP.setVisible(false);
					jLabelPort.setVisible(false);
					jTextFieldIP.setVisible(false);
					jTextFieldPort.setVisible(false);
					if(jCheckBoxUART.isSelected()){
						jCheckBoxUART.setSelected(true);
					}else{
						jCheckBoxETHERNET.setSelected(false);
						jCheckBoxUART.setSelected(true);
					}
				}
				public void mousePressed(java.awt.event.MouseEvent e) {
					if(jCheckBoxUART.isSelected()){
						jCheckBoxUART.setSelected(true);
					}else{
						jCheckBoxETHERNET.setSelected(false);
						jCheckBoxUART.setSelected(true);
					}
				}
				public void mouseReleased(java.awt.event.MouseEvent e) {
				}
				public void mouseEntered(java.awt.event.MouseEvent e) {
				}
				public void mouseExited(java.awt.event.MouseEvent e) {
				}
			});
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
			jCheckBoxETHERNET.addMouseListener(new java.awt.event.MouseListener() {
				public void mouseClicked(java.awt.event.MouseEvent e) {
					choice.setVisible(false);
					jLabel1IP.setVisible(true);
					jLabelPort.setVisible(true);
					jTextFieldIP.setVisible(true);
					jTextFieldPort.setVisible(true);
					if(jCheckBoxETHERNET.isSelected()){
						jCheckBoxETHERNET.setSelected(true);
					}else{
						
						jCheckBoxUART.setSelected(false);
						jCheckBoxETHERNET.setSelected(true);
						
					}
				}
				public void mousePressed(java.awt.event.MouseEvent e) {
					if(jCheckBoxETHERNET.isSelected()){
						jCheckBoxETHERNET.setSelected(true);
					}else{
						jCheckBoxUART.setSelected(false);
						jCheckBoxETHERNET.setSelected(true);
						
					}
				}
				public void mouseReleased(java.awt.event.MouseEvent e) {
				}
				public void mouseEntered(java.awt.event.MouseEvent e) {
				}
				public void mouseExited(java.awt.event.MouseEvent e) {
				}
			});
		}
		return jCheckBoxETHERNET;
	}

	/**
	 * This method initializes jButtonOK	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButtonOK() {
		if (jButtonOK == null) {
			jButtonOK = new JButton();
			jButtonOK.setBounds(new Rectangle(38, 291, 81, 41));
			jButtonOK.setText("OK");
			jButtonOK.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					RevData _testGetData = new RevData();
					byte[] byteArray = new byte[] { 69, 57, 48, 49, 50, 51, 52, 53, 48, 
                            							57, 49,	50, 51, 52, 53, 54, 55
													};
					String[] _a = _testGetData.ConvertDataFromBoard(byteArray, byteArray.length);
					String[] _b= _testGetData.GetDataFromDatabase(PathDatabase, _a, _a.length);
					jLabel1PID.setText(_b[0]);
					String space1="         ";
					String space2="                                  ";
					String space3="                                          ";
					String space4="                                                                                                        ";
					String __="-----------------------------------------------------------------------------------------------------------------";
					int t=0,i,total=0;
					String [] _c= new String[((_b.length-1)/3)+2];
					for(i=0; i<(_b.length-1)/3; i++){
						_c[i]= (i+1) + space1+ 
						       _b[++t]+ space2+ 
						       _b[++t]+space3+ 
						       _b[++t];
						total += Integer.parseInt(_b[t]);
					}
					_c[(_b.length-1)/3]=__;
					_c[(_b.length-1)/3+1]=space1+ "Total: "+space4+ total;
					jList.setListData(_c);
					WriteFile _testWrite = new WriteFile();
					try {
						String Path = _testGetData.GerneralPathToWrite(_b[0]);
						_testWrite.writeExcel(Path, _b, _b.length);
					} catch (WriteException e1) {
						// TODO Auto-generated catch block
						System.out.println("File path error");;
					} catch (IOException e2) {
						// TODO Auto-generated catch block
						e2.printStackTrace();
					}
				}
			});
		}
		return jButtonOK;
	}

	/**
	 * This method initializes choice	
	 * 	
	 * @return java.awt.Choice	
	 */
	private Choice getChoice() {
		if (choice == null) {
			choice = new Choice();
			choice.setBounds(new Rectangle(310, 345, 71, 38));
			choice.add("COM1");
			choice.add("COM2");
			choice.add("COM3");
			choice.add("COM4");
			choice.setVisible(true);
			
			
		}
		return choice;
	}

	/**
	 * This method initializes jList	
	 * 	
	 * @return javax.swing.JList	
	 */
	private JList getJList() {
		if (jList == null) {
			jList = new JList();
		}
		return jList;
	}
	public static void main(String arg[]) {
		MainGui2 a = new MainGui2(); 
	}

//	public static void main(String[] args) {
//		//Start the threads
//	
//		//System.out.println(Thread.currentThread());
//	}

}  //  @jve:decl-index=0:visual-constraint="130,24"
