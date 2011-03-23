/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.waitbar;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;

import org.scilab.modules.gui.waitbar.SimpleWaitBar;

/**
 * Swing implementation of a Scilab Wait Bars
 * @author Vincent COUVERT
 */
public class SwingScilabWaitBar extends JFrame implements SimpleWaitBar {
	
	private static final long serialVersionUID = -5208590743368628657L;

	private static final String SPACE = " ";
	
	private static final int WIDTH = 200;
	private static final int HEIGHT = 150;
	
	private int elementId;
	
	private JProgressBar progressBar;
	
	private JLabel messageLabel;

	private ImageIcon scilabIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
	
	/**
	 * Default constructor
	 */
	public SwingScilabWaitBar() {
		super();

                setIconImage(scilabIcon.getImage());
		
        GridBagLayout gridbag = new GridBagLayout();
        GridBagConstraints c = new GridBagConstraints();
        
        JPanel pane = new JPanel();
		pane.setLayout(gridbag);
		setContentPane(pane);
		
		/* Scilab icon */
		c.gridwidth = 2;
		c.gridheight = 2;
		c.weighty = 1.0;
		JLabel icon = new JLabel();
		gridbag.setConstraints(icon, c);
		icon.setIcon(scilabIcon);
		pane.add(icon);

		/* Space between icon and text */
		JLabel emptySpace = new JLabel(SPACE);
		c.gridheight = 1;
		c.gridwidth = 1;
		gridbag.setConstraints(emptySpace, c);
		pane.add(emptySpace);

		/* Message */
		c.gridwidth = GridBagConstraints.REMAINDER;	
		c.gridheight = 2;
		c.weighty = 0.0;
		messageLabel = new JLabel();
		gridbag.setConstraints(messageLabel, c);
		pane.add(messageLabel);
		
		/* Empty space between message and progress bar */
		emptySpace = new JLabel(SPACE);
		c.gridheight = 1;
		c.gridwidth = GridBagConstraints.REMAINDER;
		gridbag.setConstraints(emptySpace, c);
		pane.add(emptySpace);
		
		/* ProgressBar */
		progressBar = new JProgressBar();
		c.gridwidth = GridBagConstraints.REMAINDER;
		gridbag.setConstraints(progressBar, c);
		pane.add(progressBar);
		
		/* Bottom empty space */
		JLabel bottomEmptySpace = new JLabel(SPACE);
		c.gridwidth = GridBagConstraints.REMAINDER;
		gridbag.setConstraints(bottomEmptySpace, c);
		pane.add(bottomEmptySpace);
		
		this.setSize(WIDTH, HEIGHT);
		this.setVisible(true);
		this.doLayout();
		
	}
	
	/**
	 * Set the element id for this MessageBox
	 * @param id the id of the corresponding MessageBox object
	 */
	public void setElementId(int id) {
		elementId = id;
	}
	
	/**
	 * Get the element id for this MessageBox
	 * @return id the id of the corresponding MessageBox object
	 */
	public int getElementId() {
		return elementId;
	}
	
	/**
	 * Set the title of the WaitBar
	 * @param title the title to set
	 */
	public void setTitle(String title) {
		super.setTitle(title);
	}
	
	/**
	 * Set the message of the WaitBar
	 * @param message the message to set
	 */
	public void setMessage(String[] message) {
		int line = 0;
		StringBuffer msg = new StringBuffer("<HTML>");
		for (line = 0; line < message.length - 1; line++) {
			msg.append(message[line] + "<br>");
		}
		msg.append(message[line] + "</HTML>");
		messageLabel.setText(msg.toString());
	}
	
	/**
	 * Set the current value of the WaitBar
	 * @param value the value to set
	 */
	public void setValue(int value) {
		progressBar.setValue(value);
	}
	
	/**
	 * Close the WaitBar
	 */
	public void close() {
		this.dispose();
	}
	
	/**
	 * Indicates if the total execution time is known
	 * @param status true if the total progress time in unknown
	 */
	public void setIndeterminateMode(boolean status) {
		progressBar.setIndeterminate(status);
		progressBar.setStringPainted(!status);

	}

}
