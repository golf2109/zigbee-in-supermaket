package com.Gui;

import java.awt.Choice;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.SystemColor;
import java.awt.Toolkit;
import java.awt.event.KeyEvent;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URL;
import java.util.Enumeration;

import javax.comm.CommPortIdentifier;
import javax.comm.SerialPort;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.border.BevelBorder;
import javax.swing.border.TitledBorder;
import javax.swing.table.JTableHeader;

import com.ConvertDataIn;
import com.Uart.Read;

public class MainGui extends JFrame {

	private static final long serialVersionUID = 1L;
	JPanel jContentPane = null;  //  @jve:decl-index=0:visual-constraint="145,69"
	JButton jButtonConnect = null;
	JLabel jLabelPort = null;
	JTextField jTextFieldPort = null;
	JButton jButtonDisconnect = null;
	static JButton jClearButton = null;
	Thread thread1;
	Socket skt;
	
	private JLabel jLabel1IP = null;
	private JTextField jTextFieldIP = null;
	static String PathDatabase ="c:/temp/Database.xls";  //  @jve:decl-index=0:
	private static JButton jPrintButton = null;
	private Choice choice = null;
	private static JLabel jLabel1PacketID = null;
	private static JLabel jLabel1PID = null;
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
	public static String [][] _d= new String[50][500];
	static int length_d=0;
	public static int numofpacket=0;
	public static int[] NumOfNumProduct = new int[500];
	Read reader =null;
	public static ErrorGui StatusGui = null;
	public static Help HelpGui = null;
	private JButton jButtonHelp = null;
	private JPanel jCOMPanel = null;
	private JRadioButton jUARTRadioButton = null;
	private JRadioButton jEthernetRadioButton = null;
	private JPanel jEthernetPanel = null;
	private JScrollPane jTableScrollPane = null;
	private static JTable jTable = null;
	JTextPane jTextPaneHcmut =null;
	JTextPane jTextPaneQueueBrusting =null;
	JTextPane jTextPaneDatalogic =null;
	static String [][] _c= new String[500][5];
	static String col[] = {"Index","ProductID","ProductName","Price","NumOfProduct"};
	/**
	 * This is the default constructor
	 */
	public MainGui() {
		super();
		try {
			// Set System L&F
			UIManager.setLookAndFeel(
			UIManager.getSystemLookAndFeelClassName());
			} 
			catch (UnsupportedLookAndFeelException e) {
			// handle exception
			}
			catch (ClassNotFoundException e) {
			// handle exception
			}
			catch (InstantiationException e) {
			// handle exception
			}
			catch (IllegalAccessException e) {
			// handle exception
			}
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
		this.setSize(537, 557);
		this.setBackground(new Color(255, 102, 0));
		this.setTitle("Casher");
		this.setVisible(true);
		this.setLocation(350, 70);
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
			jLabel1Money.setBounds(new Rectangle(396, 340, 115, 31));
			jLabel1Money.setText("");
			jLabel1Total = new JLabel();
			jLabel1Total.setBounds(new Rectangle(300, 340, 96, 31));
			jLabel1Total.setText("");
			jLabel1text = new JLabel();
			jLabel1text.setBounds(new Rectangle(300, 400, 211, 30));
			jLabel1text.setText("");
			jLabel1PID = new JLabel();
			jLabel1PID.setBounds(new Rectangle(120, 340, 123, 31));
			jLabel1PID.setText("");
			jLabel1PacketID = new JLabel();
			jLabel1PacketID.setBounds(new Rectangle(10, 340, 81, 31));
			jLabel1PacketID.setText("Packet Id:");
			jLabel1PacketID.setVisible(false);
			jLabel1IP = new JLabel();
			jLabel1IP.setText("IP:");
			jLabel1IP.setDisplayedMnemonic(KeyEvent.VK_UNDEFINED);
			jLabel1IP.setBounds(new Rectangle(10, 30, 19, 31));
			
			jLabel1IP.setVisible(true);
			jLabelPort = new JLabel();
			jLabelPort.setText("Port:");
			jLabelPort.setBounds(new Rectangle(150, 30, 27, 31));
			jLabelPort.setVisible(true);
			jContentPane = new JPanel();
			jContentPane.setLayout(null);
			jContentPane.setBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED));
			jContentPane.setForeground(new Color(74, 91, 94));
			jContentPane.add(getJClearButton(), null);
			jContentPane.add(getJPrintButton(), null);
			jContentPane.add(jLabel1PacketID, null);
			jContentPane.add(jLabel1PID, null);
			jContentPane.add(jLabel1text, null);
			jContentPane.add(jLabel1Total, null);
			jContentPane.add(jLabel1Money, null);
			jContentPane.add(getJButtonHelp(), null);
			jContentPane.add(getJUARTRadioButton(), null);
			jContentPane.add(getJEthernetRadioButton(), null);
			jContentPane.add(getJEthernetPanel(), null);
			jContentPane.add(getJButtonDisconnect(), null);
			jContentPane.add(getJButtonConnect(), null);
			jContentPane.add(getJCOMPanel(), null);
			jContentPane.add(getJTableScrollPane(), null);
			jContentPane.add(getJTextPaneHcmut(), null);
			jContentPane.add(getJTextPaneQueueBrusting(), null);
			jContentPane.add(getJTextPaneDatalogic(), null);
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
			jButtonConnect.setText("Connect");
			jButtonConnect.setBounds(new Rectangle(10, 480, 101, 31));
			jButtonConnect.setVisible(true);
			jButtonConnect.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("actionPerformed()"); // TODO Auto-generated Event stub actionPerformed()
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
									ErrorGui.Type = "error";
									ErrorGui.content = choice.getSelectedItem()+ " is used by another program";
								}
								else{
									jLabel1text.setText("Receive Data From "+ choice.getSelectedItem());
									choice.setEnabled(false);
									jButtonConnect.setEnabled(false);
									jButtonDisconnect.setEnabled(true);
									jPrintButton.setEnabled(true);
									jClearButton.setEnabled(true);
									jButtonCheckComPort.setEnabled(false);
									ErrorGui.Type = "message";
									ErrorGui.content = "Begin Receive Data From "+ choice.getSelectedItem();
								}
							}
						}
					}
					if (!portFound) {
						jLabel1text.setText("Port not found.");
						ErrorGui.Type = "error";
						ErrorGui.content = "Port not found";
					}
					StatusGui = new ErrorGui();
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
			jTextFieldPort.setText("123");
			jTextFieldPort.setBounds(new Rectangle(180, 30, 34, 31));
			jTextFieldPort.setVisible(true);
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
			jButtonDisconnect.setBounds(new Rectangle(140, 480, 101, 31));
			jButtonDisconnect.setVisible(true);
			jButtonDisconnect.addActionListener(new java.awt.event.ActionListener() {
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
					
					choice.setEnabled(true);
					jLabel1text.setText("");
					jButtonCheckComPort.setEnabled(true);
					jButtonConnect.setEnabled(true);
					jButtonDisconnect.setEnabled(false);
					jPrintButton.setEnabled(false);
					jClearButton.setEnabled(false);
				}
			});
		}
		return jButtonDisconnect;
	}

	/**
	 * This method initializes jClearButton
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJClearButton() {
		if (jClearButton == null) {
			jClearButton = new JButton();
			jClearButton.setBounds(new Rectangle(420, 480, 91, 31));
			jClearButton.setText("Clear");
			jClearButton.setEnabled(false);
			jClearButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					
					JTableHeader header = jTable.getTableHeader();
					header.setBackground(Color.blue);
					for(int i=0; i<500; i++)
						for(int j=0; j<5; j++)
							_c[i][j]="";
					
					jTable.updateUI();
					numofline =0;
					numofpacket =0;
					total =0;
			        jLabel1Total.setText(null);
			        jLabel1Money.setText(null);

				}
			});
		}
		return jClearButton;
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
	 * This method initializes jTextFieldIP	
	 * 	
	 * @return javax.swing.JTextField	
	 */
	private JTextField getJTextFieldIP() {
		if (jTextFieldIP == null) {
			jTextFieldIP = new JTextField();
			jTextFieldIP.setText("192.168.13.12");
			jTextFieldIP.setBounds(new Rectangle(30, 30, 101, 31));
			jTextFieldIP.setVisible(true);
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
	 * This method initializes jPrintButton	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJPrintButton() {
		if (jPrintButton == null) {
			jPrintButton = new JButton();
			jPrintButton.setBounds(new Rectangle(420, 440, 91, 31));
			                             //     (295, 200, 100, 29)
			jPrintButton.setText("Print");
			jPrintButton.setEnabled(false);
			jPrintButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					;
				}
			});
		}
		return jPrintButton;
	}

	/**
	 * This method initializes choice	
	 * 	
	 * @return java.awt.Choice	
	 */
	private Choice getChoice() {
		if (choice == null) {
			choice = new Choice();
			choice.setVisible(true);
			choice.setSize(new Dimension(61, 21));
			choice.setLocation(new Point(15, 30));
			
			
		}
		return choice;
	}

	public static void ProcessData(int _iType){
    	 	ConvertDataIn _testGetData = new ConvertDataIn();
			int t=0;
			int i;
			int _total = 0;
			int numofproduct=0;
			String _a;
			String[] _b;
			if(_iType ==0){
				_a = _testGetData.ConvertDataFromBoard(Read.readBuffer, 0);
				_b = _testGetData.GetDataFromDatabase(PathDatabase, _a, 0);
				//jLabel1PID.setText(_b[0]);
				length_d=0;
				_c[numofline++][0] = "Packet ID:  "+_b[0];
				_d[numofpacket][length_d++] =_b[0];
				for(i=0; i<(_testGetData.iNumOfTypeProduct)/4; i++){
					numofproduct = 0;
					_d[numofpacket][length_d++] =_b[t+1];
					_d[numofpacket][length_d++] =_b[t+2];
					_d[numofpacket][length_d++] =_b[t+3];
					_d[numofpacket][length_d++] =_b[t+4];//for write
					
					_c[numofline][numofproduct++] =Integer.toString(i+1);
					_c[numofline][numofproduct++] =_b[++t];
					_c[numofline][numofproduct++] =_b[++t];
					_c[numofline][numofproduct++] =_b[++t];
					_c[numofline++][numofproduct++] =_b[++t];
					
					_total += Integer.parseInt(_b[t-1])*Integer.parseInt(_b[t]); // total price each packet
					
					
				}
				_c[++numofline][3] ="Total";
				_c[numofline++][4] =_total+ "";
				
				total += _total; // total price all packet
				JTableHeader header = jTable.getTableHeader();
				header.setBackground(Color.blue);
				jTable.updateUI();
				
				jLabel1Total.setText("Total: ");
				jLabel1Money.setText(total+" ");
				NumOfNumProduct[numofpacket++]=_testGetData.iNumOfTypeProduct;
				jPrintButton.setEnabled(true);
		        jClearButton.setEnabled(true);
			}else{
				_a = _testGetData.ConvertDataFromBoard(Read.readBuffer, 1);
				if(_a.substring(0, 1).equals("+")||_a.substring(0, 1).equals("-")){
				_b = _testGetData.GetDataFromDatabase(PathDatabase, _a, 1);
				jLabel1PID.setText(_b[0]);
				length_d=0;
				_c[numofline][0] =_b[0];
				_c[numofline][1] =_b[1];
				
				if(_b[0].equals("Not Found"))
				{
					JTableHeader header = jTable.getTableHeader();
					header.setBackground(Color.red);
					System.out.println("Not in database");
					numofline++;
				}else{
				JTableHeader header = jTable.getTableHeader();
				header.setBackground(Color.blue);
				_d[numofpacket][0] =_b[0];
				_d[numofpacket][1] =_b[1];
				_d[numofpacket][2] =_b[2];
				_d[numofpacket][3] =_b[3];
				_d[numofpacket][4] =_b[4];//for write
				
				_c[numofline][1] =_b[1];
				_c[numofline][2] = _b[2];
				_c[numofline][3] = _b[3];
				_c[numofline++][4] = _b[4];
				
				_total += Integer.parseInt(_b[4])*Integer.parseInt(_b[3]); // total price each packet
				
				if(_b[0].equals("Add more product"))
					total += _total; // total price all packet
				else total -= _total; // total price all packet
				
				jLabel1Total.setText("Total ");
				jLabel1Money.setText(total+"");
				
				NumOfNumProduct[numofpacket++]=	1;
				}
				jTable.updateUI();
			}
		}
     }
	/**
	 * This method initializes jButtonCheckComPort	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButtonCheckComPort() {
		if (jButtonCheckComPort == null) {
			jButtonCheckComPort = new JButton();
			jButtonCheckComPort.setText("CheckPort");
			jButtonCheckComPort.setBounds(new Rectangle(120, 30, 94, 21));
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
								//jButtonGetData.setEnabled(true);
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
	 * This method initializes jTextPaneHcmut	
	 * 	
	 * @return javax.swing.JTextPane	
	 */
	private JTextPane getJTextPaneHcmut() {
		if (jTextPaneHcmut  == null) {
			jTextPaneHcmut = new JTextPane();
			jTextPaneHcmut.setBounds(new Rectangle(1, 1, 102, 96));
			jTextPaneHcmut.setBackground(SystemColor.control);
		    URL url = getClass().getResource("pic/hcmut.gif");
		    Image image = Toolkit.getDefaultToolkit().getImage(url);
		    ImageIcon icon=new ImageIcon(image);
	        jTextPaneHcmut.insertIcon(icon);
	        jTextPaneHcmut.setEditable(false);
	        jTextPaneHcmut.setVisible(true);
		}
		return jTextPaneHcmut;
	}

	/**
	 * This method initializes jTextPaneQueueBrusting	
	 * 	
	 * @return javax.swing.JTextPane	
	 */
	private JTextPane getJTextPaneQueueBrusting() {
		if (jTextPaneQueueBrusting == null) {
			jTextPaneQueueBrusting = new JTextPane();
			jTextPaneQueueBrusting.setBounds(new Rectangle(199, 42, 267, 51));
			jTextPaneQueueBrusting.setBackground(SystemColor.control);
			 URL url = getClass().getResource("pic/QueueBrusting.gif");
		     Image image = Toolkit.getDefaultToolkit().getImage(url);
		     ImageIcon icon=new ImageIcon(image);
		     jTextPaneQueueBrusting.insertIcon(icon);
		     jTextPaneQueueBrusting.setEditable(false);
		}
		return jTextPaneQueueBrusting;
	}

	/**
	 * This method initializes jTextPaneDatalogic	
	 * 	
	 * @return javax.swing.JTextPane	
	 */
	private JTextPane getJTextPaneDatalogic() {
		if (jTextPaneDatalogic == null) {
			jTextPaneDatalogic = new JTextPane();
			jTextPaneDatalogic.setBounds(new Rectangle(1, 3, 173, 76));
			jTextPaneDatalogic.setBackground(SystemColor.controlHighlight);
			 URL url = getClass().getResource("pic/Datalogic.gif");
		     Image image = Toolkit.getDefaultToolkit().getImage(url);
		     ImageIcon icon=new ImageIcon(image);
		     jTextPaneDatalogic.insertIcon(icon);
		     jTextPaneDatalogic.setEditable(false);
		     jTextPaneDatalogic.setVisible(false);
		}
		return jTextPaneDatalogic;
	}

	/**
	 * This method initializes jButtonHelp	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButtonHelp() {
		if (jButtonHelp == null) {
			jButtonHelp = new JButton();
			jButtonHelp.setBounds(new Rectangle(300, 440, 91, 30));
			jButtonHelp.setText("Help");
			jButtonHelp.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					HelpGui = new Help();
				}
			});
		}
		return jButtonHelp;
	}

	/**
	 * This method initializes jCOMPanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getJCOMPanel() {
		if (jCOMPanel == null) {
			jCOMPanel = new JPanel();
			jCOMPanel.setLayout(null);
			jCOMPanel.setBorder(BorderFactory.createTitledBorder(null, "Settings", TitledBorder.DEFAULT_JUSTIFICATION, TitledBorder.DEFAULT_POSITION, new Font("Dialog", Font.BOLD, 12), new Color(51, 51, 51)));
			jCOMPanel.setSize(new Dimension(231, 71));
			jCOMPanel.setLocation(new Point(10, 400));
			jCOMPanel.add(getJButtonCheckComPort(), null);
			jCOMPanel.add(getChoice(), null);
		}
		return jCOMPanel;
	}

	/**
	 * This method initializes jUARTRadioButton	
	 * 	
	 * @return javax.swing.JRadioButton	
	 */
	private JRadioButton getJUARTRadioButton() {
		if (jUARTRadioButton == null) {
			jUARTRadioButton = new JRadioButton();
			jUARTRadioButton.setBounds(new Rectangle(130, 380, 101, 21));
			jUARTRadioButton.setText("COM");
			jUARTRadioButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("actionPerformed()"); // TODO Auto-generated Event stub actionPerformed()
					jUARTRadioButton.setSelected(true);
					jEthernetRadioButton.setSelected(false);
					jEthernetPanel.setVisible(false);
					jCOMPanel.setVisible(true);
					jButtonConnect.setEnabled(true);
					jButtonDisconnect.setEnabled(false);
				}
			});
		}
		return jUARTRadioButton;
	}

	/**
	 * This method initializes jEthernetRadioButton	
	 * 	
	 * @return javax.swing.JRadioButton	
	 */
	private JRadioButton getJEthernetRadioButton() {
		if (jEthernetRadioButton == null) {
			jEthernetRadioButton = new JRadioButton();
			jEthernetRadioButton.setBounds(new Rectangle(30, 380, 81, 21));
			jEthernetRadioButton.setText("Ethernet");
			jEthernetRadioButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("actionPerformed()"); // TODO Auto-generated Event stub actionPerformed()
					jUARTRadioButton.setSelected(false);
					jEthernetRadioButton.setSelected(true);
					jEthernetPanel.setVisible(true);
					jCOMPanel.setVisible(false);
					jButtonConnect.setEnabled(true);
					jButtonDisconnect.setEnabled(false);
				}
			});
		}
		return jEthernetRadioButton;
	}

	/**
	 * This method initializes jEthernetPanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getJEthernetPanel() {
		if (jEthernetPanel == null) {
			jEthernetPanel = new JPanel();
			jEthernetPanel.setLayout(null);
			jEthernetPanel.setBounds(new Rectangle(10, 400, 231, 71));
			jEthernetPanel.setBorder(BorderFactory.createTitledBorder(null, "Settings", TitledBorder.DEFAULT_JUSTIFICATION, TitledBorder.DEFAULT_POSITION, new Font("Dialog", Font.BOLD, 12), new Color(51, 51, 51)));
			jEthernetPanel.setVisible(false);
			jEthernetPanel.add(jLabel1IP, null);
			jEthernetPanel.add(jLabelPort, null);
			jEthernetPanel.add(getJTextFieldPort(), null);
			jEthernetPanel.add(getJTextFieldIP(), null);
		}
		return jEthernetPanel;
	}

	/**
	 * This method initializes jTableScrollPane	
	 * 	
	 * @return javax.swing.JScrollPane	
	 */
	private JScrollPane getJTableScrollPane() {
		if (jTableScrollPane == null) {
			jTableScrollPane = new JScrollPane(getJTable());
			jTableScrollPane.setBounds(new Rectangle(15, 135, 496, 181));
			jTableScrollPane.setViewportView(getJTable());
			jTableScrollPane.setAutoscrolls(true);
			
		}
		return jTableScrollPane;
	}

	/**
	 * This method initializes jTable	
	 * 	
	 * @return javax.swing.JTable	
	 */
	private JTable getJTable() {
		if (jTable == null) {
			jTable = new JTable(_c, col);
			JTableHeader header = jTable.getTableHeader();
			header.setBackground(Color.red);
			jTable.setEnabled(false);
		}
		return jTable;
	}

	public static void main(String arg[]) {
		MainGui a = new MainGui(); 
	}
}  //  @jve:decl-index=0:visual-constraint="20,10"