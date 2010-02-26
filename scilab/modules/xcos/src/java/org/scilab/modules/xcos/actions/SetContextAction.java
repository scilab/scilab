/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Opens context settings Window
 */
public class SetContextAction extends SimulationNotRunningAction {
	public static final String NAME = XcosMessages.SET_CONTEXT;
	public static final String SMALL_ICON = "";
	public static final int MNEMONIC_KEY = 0;
	public static final int ACCELERATOR_KEY = 0;
	
	private XcosDiagram diagram;
	private JFrame mainFrame;
	private JTextArea contextArea;
	private boolean windowAlreadyExist;
	
	/**
	 * Constructor
	 * @param scilabGraph corresponding Scilab Graph
	 */
	public SetContextAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}
	
	/**
	 * Create the associated menu
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		SetContextAction action = new SetContextAction(scilabGraph);
		((XcosDiagram) scilabGraph).setContextAction(action);
		return createMenu(scilabGraph, SetContextAction.class);
	}
	
	/**
	 * Action !
	 * @param e the event
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		setContextBox(e);
	}

	/**
	 * Window creation
	 * @param e the event
	 */
	public void setContextBox(ActionEvent e) {
		
		/** Avoid to have this window created two times */
		if (windowAlreadyExist) {
			mainFrame.setVisible(true);
			return;
		}
		
		Icon scilabIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
		Image imageForIcon = ((ImageIcon) scilabIcon).getImage();

		diagram = (XcosDiagram) getGraph(e);
        
		mainFrame = new JFrame();
        windowAlreadyExist = true;
        
        mainFrame.setLayout(new GridBagLayout());
        mainFrame.setIconImage(imageForIcon);
        
        JLabel textLabel = new JLabel(XcosMessages.SET_CONTEXT_LABEL_TEXT);
        StringBuilder contextBuilder = new StringBuilder();
        for (int i = 0; i < diagram.getContext().length; i++) {
        	contextBuilder.append(diagram.getContext()[i]);
        	contextBuilder.append(System.getProperty("line.separator"));
        }
        
        contextArea = new JTextArea(contextBuilder.toString());
      
        JScrollPane contextAreaScroll = new JScrollPane(contextArea, 
        		JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        
		JButton cancelButton = new JButton(XcosMessages.CANCEL);
		JButton okButton = new JButton(XcosMessages.OK);
		okButton.setPreferredSize(cancelButton.getPreferredSize());
		
        JPanel buttonPane = new JPanel();
        buttonPane.setLayout(new BoxLayout(buttonPane, BoxLayout.LINE_AXIS));
        buttonPane.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));
		buttonPane.add(okButton);
		buttonPane.add(Box.createRigidArea(new Dimension(10, 0)));
		buttonPane.add(cancelButton);
		
		GridBagConstraints gbc = new GridBagConstraints();
		
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbc.gridx = 0;
		gbc.gridy = 1;
		gbc.fill = GridBagConstraints.BOTH;
		gbc.weightx = 1.0;
		gbc.insets = new Insets(10, 10, 10, 10);
		
		mainFrame.add(textLabel, gbc);
		
		gbc.gridy = GridBagConstraints.RELATIVE;
		gbc.fill = GridBagConstraints.BOTH;
		gbc.weighty = 1.0;
		mainFrame.add(contextAreaScroll, gbc);
		


		gbc.gridheight = 1;
		gbc.gridwidth = 1;
		
		gbc.anchor = GridBagConstraints.LAST_LINE_END;
        gbc.fill = GridBagConstraints.NONE;
        gbc.weighty = 0;
		gbc.insets = new Insets(5, 0, 10, 10);
		mainFrame.add(buttonPane, gbc);
		
		
		cancelButton.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				windowAlreadyExist = false;
				mainFrame.dispose();
			}
		});
        
		okButton.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				ArrayList<String> contextList = new ArrayList<String>();
				int i = 0;
				try {
					StringReader stringReader = new StringReader(contextArea.getText());
					BufferedReader bufferReader = new BufferedReader(stringReader);
					String nextLine = "";

					while ((nextLine = bufferReader.readLine()) != null) {
						contextList.add(nextLine);
						i++;
					}

				} catch (IOException e1) {
					e1.printStackTrace();
				}
				/** Test for modifications */
				String[] oldContext = diagram.getContext();
				boolean modified = false;
				/* I more or less lines --> modified */
				if (oldContext.length != i) {
					modified = true;
				} else {
					/* Compare line to line */
					for (int lineNumber = 0; lineNumber < oldContext.length; lineNumber++) {
						if (!oldContext[lineNumber].equals(contextList.get(lineNumber))) {
							modified = true;
						}
					}
				}
				if (modified) {
					if (i == 0) { /* Empty context */
						diagram.setContext(new String[]{""});
					} else {
						diagram.setContext(contextList.toArray(new String[i]));
					}
					diagram.setModified(true);
				}
				windowAlreadyExist = false;
				mainFrame.dispose();
			}
		});
		
		
		mainFrame.setMinimumSize(textLabel.getPreferredSize());
        mainFrame.setTitle(XcosMessages.SET_CONTEXT);
        mainFrame.pack();
        mainFrame.setLocationRelativeTo(diagram.getAsComponent());
        mainFrame.setVisible(true);	
	}
}
