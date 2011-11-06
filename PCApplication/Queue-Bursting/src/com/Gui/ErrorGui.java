package com.Gui;

import java.awt.Rectangle;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

public class ErrorGui extends JFrame {

	private static final long serialVersionUID = 1L;
	private JPanel jContentPane = null;
	private JLabel jLabelError = null;
	private JButton jButtonOK = null;
	private JLabel jLabelErro = null;
	public static String Type = "error";
	public static String content = "error message here";
	/**
	 * This is the default constructor
	 */
	public ErrorGui() {
		super();
		initialize();
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setSize(231, 118);
		this.setContentPane(getJContentPane());
		this.setVisible(true);
		this.setAlwaysOnTop(true);
		this.setLocation(460, 320);
		this.setResizable(false);
		this.setTitle(Type);
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJContentPane() {
		if (jContentPane == null) {
			jLabelErro = new JLabel();
			jLabelErro.setBounds(new Rectangle(5, 8, 219, 39));
			jLabelErro.setHorizontalAlignment(SwingConstants.CENTER);
			jLabelErro.setHorizontalTextPosition(SwingConstants.CENTER);
			jLabelErro.setText(content);
			jLabelError = new JLabel();
			jLabelError.setText("");
			jLabelError.setBounds(new Rectangle(0, 2, 216, 46));
			jContentPane = new JPanel();
			jContentPane.setLayout(null);
			jContentPane.add(jLabelError);
			jContentPane.add(getJButtonOK(), null);
			jContentPane.add(jLabelErro, null);
		}
		return jContentPane;
	}

	/**
	 * This method initializes jButtonOK	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButtonOK() {
		if (jButtonOK == null) {
			jButtonOK = new JButton();
			jButtonOK.setBounds(new Rectangle(73, 48, 86, 30));
			jButtonOK.setText("OK");
			jButtonOK.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					MainGui.StatusGui.setVisible(false);
				}
			});
		}
		return jButtonOK;
	}



}  //  @jve:decl-index=0:visual-constraint="10,10"
