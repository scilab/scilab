/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.bridge.filechooser;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JRadioButton;

import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * Window in which we can configure option for the selected format 
 * @author Sylvestre Koumar
 *
 */
public class ExportOptionWindow extends JFrame implements ActionListener {
	
	private static final long serialVersionUID = 1L;
	
	private ExportData exportData;
	private JFrame optionFrame;
    private JRadioButton portrait;
    private JRadioButton landscape;    
    private JButton confirmOption;
    private JButton abortOption;
	
	
    /**
     * Default constructor
     * @param exportData information about the export
     */
	public ExportOptionWindow(ExportData exportData) {
		this.exportData = exportData;		
		displayOptionWindow();
		landscapePortraitOption();
		
	}

	/**
	 * Display the option window
	 */
	public void displayOptionWindow() {
		optionFrame = new JFrame("Option for " + exportData.getExportExtension().toUpperCase() + " format");
		optionFrame.setIconImage(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage());
		//Center the frame
		optionFrame.setLocationRelativeTo(optionFrame.getParent());	
		
		optionFrame.setResizable(false);
		optionFrame.setVisible(true);		
	}	
	
	/**
	 * Selection between portrait or landscape option
	 */
	public void landscapePortraitOption() {
	    
	    portrait = new JRadioButton("Portrait", true);
	    landscape = new JRadioButton("Landscape", false);

	    ButtonGroup bgroup = new ButtonGroup();
	    bgroup.add(portrait);
	    bgroup.add(landscape);
	    
	    confirmOption = new JButton("Confirm");
	    abortOption = new JButton("Abort");
	    

	    JPanel mainPanel = new JPanel();
	    BorderLayout layout = new BorderLayout();
	    mainPanel.setLayout(layout);
	    
	    JPanel center = new JPanel();
	    center.setLayout(new GridLayout(2, 1));
	    center.setBorder(BorderFactory.createTitledBorder(
	               BorderFactory.createEtchedBorder(), " Orientation "));
	    
	    center.add(portrait);
	    center.add(landscape);
	    
	    JPanel south = new JPanel();
	    
	    south.add(confirmOption);
	    south.add(abortOption);
	    
	    mainPanel.add(center, BorderLayout.CENTER);
	    mainPanel.add(south, BorderLayout.EAST);
	    
	    optionFrame.add(mainPanel);	    
	    optionFrame.pack();
	    
	    portrait.addActionListener(this);
	    landscape.addActionListener(this);
	    confirmOption.addActionListener(this);
	    abortOption.addActionListener(this);

	}
	
	/**
	 * Action manager 
	 * @param evt ActionEvent
	 */
    public void actionPerformed(ActionEvent evt) {
    	
    	boolean b = portrait.isSelected();    	
    	Vector<String> properties = new Vector<String>();
    	
    	
    	if (evt.getSource() == confirmOption) {
        	if (b) {
        		properties.add(portrait.getText().toLowerCase());
    		} else {
    			properties.add(landscape.getText().toLowerCase());
    		}  
			exportData.setExportProperties(properties);
			optionFrame.dispose();
			
			
			//now we manage only one option (portrait/landscape)
			//later we will have to manage more options (Vector<String> properties)
			String exportcmd = "xs2" + exportData.getExportExtension() + "(" + exportData.getFigureId() + ", '" + exportData.getExportName()
			+ "', '" + exportData.getExportProperties().elementAt(0) + "');";
			
			InterpreterManagement.putCommandInScilabQueue(exportcmd);
		}
    	
    	if (evt.getSource() == abortOption) {
    		optionFrame.dispose();
    	}
    	
    	
    }
	
	
}
