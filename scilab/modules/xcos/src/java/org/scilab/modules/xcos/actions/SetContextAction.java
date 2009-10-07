/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

public class SetContextAction extends DefaultAction {
	
	private static final long serialVersionUID = 1L;
	
	private XcosDiagram diagram;
	private static JFrame mainFrame;
	private JTextArea contextArea;
	
	/**
	 * Constructor
	 * @param scilabGraph corresponding Scilab Graph
	 */
	public SetContextAction(ScilabGraph scilabGraph) {
		super(XcosMessages.SET_CONTEXT, scilabGraph);
	}
	
	public void actionPerformed(ActionEvent e) {
		setContextBox(e);
	}

	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.SET_CONTEXT, null, new SetContextAction(scilabGraph), null);
	}
	
	public void setContextBox(ActionEvent e){
		diagram = (XcosDiagram)getGraph(e);

        mainFrame = new JFrame();
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainFrame.setLayout(new GridBagLayout());

        JLabel textLabel = new JLabel(XcosMessages.SET_CONTEXT_LABEL_TEXT);
        
        contextArea = new JTextArea(diagram.getContext());
        JScrollPane contextAreaScroll = new JScrollPane(contextArea, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
        															 JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        
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
		


		gbc.gridheight = gbc.gridwidth = 1;
		
		gbc.anchor = GridBagConstraints.LAST_LINE_END;
        gbc.fill = GridBagConstraints.NONE;
        gbc.weighty = 0;
		gbc.insets = new Insets(5, 0, 10, 10);
		mainFrame.add(buttonPane, gbc);
		
		
		cancelButton.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				mainFrame.dispose();
			}
		});
        
		okButton.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				diagram.setContext(contextArea.getText());
				mainFrame.dispose();
			}
		});
		
		
		mainFrame.setMinimumSize(textLabel.getPreferredSize());
        mainFrame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        mainFrame.setTitle(XcosMessages.SET_CONTEXT);
        mainFrame.pack();
        mainFrame.setLocationRelativeTo(null);
        mainFrame.setVisible(true);	
	}
}
