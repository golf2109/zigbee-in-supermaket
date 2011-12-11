package com.Gui;

import java.awt.BorderLayout;
import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

import java.awt.Dimension;
import javax.swing.JScrollPane;
import java.awt.Rectangle;
import javax.swing.JTable;
import javax.swing.table.JTableHeader;
import javax.swing.JButton;

import com.ConvertDataIn;
import com.Uart.Read;
import com.Uart.Write;

public class NetworkGui extends JFrame {

	private static final long serialVersionUID = 1L;
	private JPanel jContentPane = null;
	private JScrollPane jTableScrollPane = null;
	public static JTable jTableNetwork = null;
	static String [][] 	_c = new String[100][5];
	static String col[] = {"Index","MacAdd","ShortAdd","ParentAdd","ID"};
	private JButton jButtonRefresh = null;
	static String PathMactable ="c:/temp/MacTable.xls";  //  @jve:decl-index=0:
	static int Index = 0;
	public static NetworkGui NetGui = null;

	/**
	 * This is the default constructor
	 */
	public NetworkGui() {
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

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setSize(597, 312);
		this.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
		this.setBackground(new Color(255, 102, 0));
		this.setTitle("Network Status");
		this.setVisible(true);
		this.setLocation(350, 10);
		this.setContentPane(getJContentPane());
		this.setResizable(false);
		this.setAlwaysOnTop(true);
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJContentPane() {
		if (jContentPane == null) {
			jContentPane = new JPanel();
			jContentPane.setLayout(null);
			jContentPane.add(getJTableScrollPane(), null);
			jContentPane.add(getJButtonRefresh(), null);
		}
		return jContentPane;
	}
	/**
	 * This method initializes jTableScrollPane	
	 * 	
	 * @return javax.swing.JScrollPane	
	 */
	private JScrollPane getJTableScrollPane() {
		if (jTableScrollPane == null) {
			jTableScrollPane = new JScrollPane(getJTable());
			jTableScrollPane.setBounds(new Rectangle(34, 18, 531, 181));
			jTableScrollPane.setBorder(BorderFactory.createLineBorder(Color.gray, 2));
			jTableScrollPane.setViewportView(getJTable());
			jTableScrollPane.setWheelScrollingEnabled(true);
		
			
		}
		return jTableScrollPane;
	}

	/**
	 * This method initializes jTable	
	 * 	
	 * @return javax.swing.JTable	
	 */
	private JTable getJTable() {
		if (jTableNetwork == null) {
			jTableNetwork = new JTable(_c, col);
			JTableHeader header = jTableNetwork.getTableHeader();
			header.setBackground(Color.red);
			jTableNetwork.setEnabled(false);
			jTableNetwork.setShowGrid(true);
			jTableNetwork.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
			jTableNetwork.setVisible(true);
		}
		return jTableNetwork;
	}

	/**
	 * This method initializes jButtonRefresh	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButtonRefresh() {
		if (jButtonRefresh == null) {
			jButtonRefresh = new JButton();
			jButtonRefresh.setBounds(new Rectangle(407, 232, 113, 33));
			jButtonRefresh.setText("Refresh");
			jButtonRefresh.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					for(int i=0; i < 100; i++)
						for(int j=0; j<5; j++)
							_c[i][j]="";
					Index = 0;
					jTableNetwork.updateUI();
					if(Read.serialPort != null){
						Write.SendData("S");
					}else{
						ErrorGui.Type = "Error";
						ErrorGui.content1 = "Please connect com port";
						MainGui.StatusGui = new ErrorGui();
					}
				}
			});
		}
		return jButtonRefresh;
	}
	public static void HandleStatus(){
		ConvertDataIn _testGetData = new ConvertDataIn();
		String _a = _testGetData.ConvertDataFromBoard(Read.readBuffer, 3);
		String []_b = _testGetData.GetDataFromDatabase(PathMactable, _a, 2);
		
		_c[Index][0] = Index+1 + "";
		for(int i = 0; i < 4; i++){
			_c[Index][i+1] = _b[i];
		}
		Index++;
		if(jTableNetwork != null)
			jTableNetwork.updateUI();
		
		
	}
	public static void main(String arg[]) {
		NetGui = new NetworkGui(); 
	}
	}  
