package com.Gui;

import java.awt.Choice;
import java.awt.Rectangle;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Enumeration;

import javax.comm.CommPortIdentifier;
import javax.comm.SerialPort;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;

import jxl.write.WriteException;

import com.ConvertDataIn;
import com.Excel.WriteFile;
import com.Uart.Read;
import java.awt.Color;
import javax.swing.BorderFactory;
import javax.swing.border.EtchedBorder;
import javax.swing.border.BevelBorder;
import java.awt.Dimension;

public class MainGui1 extends JFrame {

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
	static String PathDatabase ="c:/temp/Database.xls";  //  @jve:decl-index=0:
	private JCheckBox jCheckBoxUART = null;
	private JCheckBox jCheckBoxETHERNET = null;
	private static JButton jButtonOK = null;
	private Choice choice = null;
	private static JList jList = null;
	private JLabel jLabel1PacketID = null;
	private static JLabel jLabel1PID = null;
	private JButton jButtonSendCom = null;
	private JLabel jLabel1text = null;
	
	String HostIP;
	int port;
	static CommPortIdentifier portId;
	static Enumeration portList;
	InputStream inputStream;
	SerialPort serialPort;
	int numBytes;
	byte[] readBuffer = new byte[17];
	private JButton jButtonCheckComPort = null;
	static int t=0;
	static String [] Data=new String[500];
	private static JLabel jLabel1Total = null;
	private static JLabel jLabel1Money = null;
	static int total = 0;
	static int numofline=0;
	static String [] _c= new String[500];
	public static String [][] _d= new String[50][500];
	static int length_d=0;
	public static int numofpacket=0;
	public static int[] NumOfNumProduct = new int[500];
	private JButton jButtonStop = null;
	Read reader =null;
	/**
	 * This is the default constructor
	 */
	public MainGui1() {
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
		this.setSize(601, 549);
		this.setBackground(new Color(255, 102, 0));
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
			jLabel1Money = new JLabel();
			jLabel1Money.setBounds(new Rectangle(425, 348, 99, 28));
			jLabel1Money.setText("");
			jLabel1Total = new JLabel();
			jLabel1Total.setBounds(new Rectangle(299, 351, 96, 28));
			jLabel1Total.setText("");
			jLabel1text = new JLabel();
			jLabel1text.setBounds(new Rectangle(406, 472, 169, 30));
			jLabel1text.setText("");
			jLabel1PID = new JLabel();
			jLabel1PID.setBounds(new Rectangle(141, 352, 123, 27));
			jLabel1PID.setText("");
			jLabel1PacketID = new JLabel();
			jLabel1PacketID.setBounds(new Rectangle(52, 347, 81, 32));
			jLabel1PacketID.setText("Packet Id:");
			jLabel1IP = new JLabel();
			jLabel1IP.setBounds(new Rectangle(53, 428, 39, 38));
			jLabel1IP.setText("IP:");
			
			jLabel1IP.setVisible(false);
			jLabel = new JLabel(); 
			jLabel.setBounds(new Rectangle(33, 104, 492, 23));
			jLabel
					.setText("Index        ProductID                            ProductName                                       Price");
			jLabelPort = new JLabel();
			jLabelPort.setBounds(new Rectangle(52, 465, 57, 38));
			jLabelPort.setText("Port:");
			jLabelPort.setVisible(false);
			jContentPane = new JPanel();
			jContentPane.setLayout(null);
			jContentPane.setBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED));
			jContentPane.setForeground(new Color(74, 91, 94));
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
			jContentPane.add(getJButtonSendCom(), null);
			jContentPane.add(jLabel1text, null);
			jContentPane.add(getJButtonCheckComPort(), null);
			jContentPane.add(jLabel1Total, null);
			jContentPane.add(jLabel1Money, null);
			jContentPane.add(getJButtonStop(), null);
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
			jButtonConnect.setBounds(new Rectangle(211, 424, 150, 35));
			jButtonConnect.setText("Connect");
			jButtonConnect.setVisible(false);
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
			jTextFieldPort.setBounds(new Rectangle(42, 389, 34, 39));
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
			jButtonDisconnect.setVisible(false);
			jButtonDisconnect.setBounds(new Rectangle(211, 461, 151, 34));
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
			jButtonExit.setBounds(new Rectangle(425, 440, 105, 29));
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
			jButtonClear.setBounds(new Rectangle(425, 410, 105, 30));
			jButtonClear.setText("Clear");
			jButtonClear.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					String []data={" "};
			         jList.setListData(data);
			         jLabel1PID.setText(null);
			         jLabel1Total.setText(null);
			         jLabel1Money.setText(null);
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
						System.out.print("Sending string: " + data + "'\n");
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
			jScrollPane.setBounds(new Rectangle(33, 128, 511, 213));
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
			jTextFieldIP.setBounds(new Rectangle(26, 353, 113, 37));
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
			jCheckBoxUART.setBounds(new Rectangle(59, 399, 73, 21));
			jCheckBoxUART.setText("UART");
			jCheckBoxUART.setSelected(true);
			jCheckBoxUART.addMouseListener(new java.awt.event.MouseListener() {
				public void mouseClicked(java.awt.event.MouseEvent e) {
					choice.setVisible(true);
					jLabel1IP.setVisible(false);
					jLabelPort.setVisible(false);
					jTextFieldIP.setVisible(false);
					jTextFieldPort.setVisible(false);
					jButtonConnect.setVisible(false);
					jButtonDisconnect.setVisible(false);
					jButtonCheckComPort.setVisible(true);
					jButtonSendCom.setVisible(true);
					jButtonStop.setVisible(true);
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
			jCheckBoxETHERNET.setBounds(new Rectangle(134, 402, 89, 17));
			jCheckBoxETHERNET.setText("ETHERNET");
			jCheckBoxETHERNET.addMouseListener(new java.awt.event.MouseListener() {
				public void mouseClicked(java.awt.event.MouseEvent e) {
					choice.setVisible(false);
					jLabel1IP.setVisible(true);
					jLabelPort.setVisible(true);
					jTextFieldIP.setVisible(true);
					jTextFieldPort.setVisible(true);
					jButtonConnect.setVisible(true);
					jButtonDisconnect.setVisible(true);
					jButtonCheckComPort.setVisible(false);
					jButtonSendCom.setVisible(false);
					jButtonStop.setVisible(false);
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
			jButtonOK.setBounds(new Rectangle(425, 380, 105, 29));
			jButtonOK.setText("OK");
			jButtonOK.setEnabled(false);
			jButtonOK.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					WriteFile _testWrite = new WriteFile();
					String Path = ConvertDataIn.GerneralPathToWrite(_d[0][0]);
						try {
							_testWrite.WriteExcel(Path);
						} catch (WriteException e1) {
							// TODO Auto-generated catch block
							System.out.println("File path error");;
						} catch (IOException e2) {
							// TODO Auto-generated catch block
							e2.printStackTrace();
						}
						jButtonOK.setEnabled(false);
						total = 0;
						numofline=0;
						_c= new String[500];
						_d= new String[50][500];
						length_d=0;
						numofpacket=0;
						NumOfNumProduct = new int[500];
						
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
			choice.setBounds(new Rectangle(107, 442, 95, 21));
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
	/**
	 * This method initializes jButtonSendCom	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButtonSendCom() {
		if (jButtonSendCom == null) {
			jButtonSendCom = new JButton();
			jButtonSendCom.setBounds(new Rectangle(253, 430, 128, 32));
			jButtonSendCom.setText("Recv Data ");
			jButtonSendCom.setEnabled(false);
			jButtonSendCom.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					boolean portFound = false;
					String defaultPort = choice.getSelectedItem();
					portList = CommPortIdentifier.getPortIdentifiers();

					while (portList.hasMoreElements()) {
						portId = (CommPortIdentifier) portList.nextElement();
						if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
							if (portId.getName().equals(defaultPort)) {
								portFound = true;
								reader = new Read(portId);
								if(Read.bError){
									jLabel1text.setText(choice.getSelectedItem()+ " is busy");
									choice.setEnabled(true);
								}
								else{
									jLabel1text.setText("Recv Data From "+ choice.getSelectedItem());
									choice.setEnabled(false);
									jButtonStop.setEnabled(true);
									jButtonCheckComPort.setEnabled(false);
								}
							}
						}
					}
					if (!portFound) {
						jLabel1text.setText("Port not found.");
					}
				}
			});
		}
		return jButtonSendCom;
	}
     public static void ProcessData(){
    	 	ConvertDataIn _testGetData = new ConvertDataIn();
			int t=0;
			int i;
			int _total = 0;
			String _a = _testGetData.ConvertDataFromBoard(Read.readBuffer);
			String[] _b = _testGetData.GetDataFromDatabase(PathDatabase, _a);
			jLabel1PID.setText(_b[0]);
			length_d=0;
			String _space1="         ";
			String _space2="                        ";
			String _space3="                                     ";
			_c[numofline++] = "Packet ID: "+_b[0];
			_d[numofpacket][length_d++] =_b[0];
			for(i=0; i<(_testGetData.iNumOfTypeProduct)/4; i++){
				_d[numofpacket][length_d++] =_b[t+1];
				_d[numofpacket][length_d++] =_b[t+2];
				_d[numofpacket][length_d++] =_b[t+3];
				_d[numofpacket][length_d++] =_b[t+4];//for write
				
				_c[numofline++]= (i+1) + _space1+ 
				       			 _b[++t]+ _space2+ 
				       			 _b[++t]+" x("+_b[t+2]+")"+ _space3+ 
				       			 _b[++t]; //for print out
				_total += Integer.parseInt(_b[t])*Integer.parseInt(_b[++t]); // total price each packet
				
				
			}
			total += _total; // total price all packet
			_c[numofline++]= "----------------------------------------------------------------------------------------------------------------";
			_c[numofline++]=_space2+ "              Total:                                                                                 "+_total;
			
			jLabel1Total.setText("Total: ");
			String _sTotal=new String();
			_sTotal=total+"";
			jLabel1Money.setText(_sTotal);
			jList.setListData(_c);
			NumOfNumProduct[numofpacket++]=_testGetData.iNumOfTypeProduct;
			jButtonOK.setEnabled(true);
//			for(i=0;i<numofpacket;i++){
//				
//				for(int j=0;j<NumOfNumProduct[i]+1;j++)
//				{					
//					System.out.print(_d[i][j]+"  ");
//				}
//				System.out.println();
//			}
//			System.out.println();
			
			
			
	//	 jButtonOK.doClick();
     }
	/**
	 * This method initializes jButtonCheckComPort	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButtonCheckComPort() {
		if (jButtonCheckComPort == null) {
			jButtonCheckComPort = new JButton();
			jButtonCheckComPort.setBounds(new Rectangle(253, 397, 127, 29));
			jButtonCheckComPort.setText("CheckPort");
			jButtonCheckComPort.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					boolean isComPort = false;
					Enumeration port_list = CommPortIdentifier.getPortIdentifiers();
					choice.removeAll();
					if(!port_list.hasMoreElements()) choice.add("not found");
					while (port_list.hasMoreElements())
					{
						CommPortIdentifier port_id = (CommPortIdentifier)port_list.nextElement();
						if (port_id.getPortType() == CommPortIdentifier.PORT_SERIAL)
							{
								isComPort = true;
								jButtonSendCom.setEnabled(true);
								choice.add(port_id.getName());
							}
					}
					if(!isComPort) jLabel1text.setText("Can't find any Com Port");
					
				}
			});
		}
		return jButtonCheckComPort;
	}

	/**
	 * This method initializes jButtonStop	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButtonStop() {
		if (jButtonStop == null) {
			jButtonStop = new JButton();
			jButtonStop.setBounds(new Rectangle(254, 467, 125, 32));
			jButtonStop.setText("Stop Recv");
			jButtonStop.setEnabled(false);
			jButtonStop.setVisible(true);
			jButtonStop.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					if(portId!=null){
						portId=null;
					}
					reader.serialPort.removeEventListener();
					reader.serialPort.close();
					reader.serialPort = null;
					try {
						reader.inputStream.close();
						reader.inputStream = null;
					} catch (IOException e2) {
						// TODO Auto-generated catch block
						e2.printStackTrace();
					}
					reader.readThread=null;
					
					jButtonStop.setEnabled(false);
					choice.setEnabled(true);
					jLabel1text.setText("");
					jButtonCheckComPort.setEnabled(true);
				}
			});
		}
		return jButtonStop;
	}

	public static void main(String arg[]) {
		MainGui1 a = new MainGui1(); 
	}
}  //  @jve:decl-index=0:visual-constraint="20,10"