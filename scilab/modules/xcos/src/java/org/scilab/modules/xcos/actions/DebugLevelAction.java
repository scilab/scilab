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

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.ListSelectionModel;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement.InterpreterException;

/**
 * Set the debug level
 */
public class DebugLevelAction extends SimulationNotRunningAction {
	public static final String NAME = XcosMessages.SET_DEBUG;
	public static final String SMALL_ICON = "";
	public static final int MNEMONIC_KEY = 0;
	public static final int ACCELERATOR_KEY = 0;

	/*
	 * FIXME: these following fields must be graph dependent instead of static.
	 * Imagine the situation when Xcos has 2 diagrams opened and perform the
	 * action.
	 */
	private static XcosDiagram diagram;
	private static JFrame mainFrame;
	private static JList debugList;

	/**
	 * @author Allan SIMON
	 *
	 */
	private enum DebugLevel {
		ZERO (0, XcosMessages.DEBUGLEVEL_0),
		ONE (1, XcosMessages.DEBUGLEVEL_1),
		TWO (2, XcosMessages.DEBUGLEVEL_2),
		THREE (3, XcosMessages.DEBUGLEVEL_3);
		
		private int level;
		private String debugName;
		
		/**
		 * @param realNumber level
		 * @param name debug name
		 */
		private DebugLevel(int realNumber, String name) {
			level = realNumber;
			debugName = name;
		}
		
		/**
		 * @return value
		 */
		public int getValue() {
			return level;
		}
		
		public String toString() {
			return debugName;
		}
	}
	
	/**
	 * Constructor
	 * @param scilabGraph corresponding Scilab Graph
	 */
	public DebugLevelAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	public void actionPerformed(ActionEvent e) {
		diagram = (XcosDiagram) getGraph(e);
		debugLevel(diagram);
	}

	/**
	 * @param scilabGraph graph
	 * @return menu item
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, DebugLevelAction.class);
	}

	/**
	 * @param diagramArgu diagram
	 */
	public static void debugLevel(XcosDiagram diagramArgu){

		diagram = diagramArgu;

		Icon scilabIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
		Image imageForIcon = ((ImageIcon) scilabIcon).getImage();

		mainFrame = new JFrame();
		mainFrame.setIconImage(imageForIcon);
		mainFrame.setLayout(new GridBagLayout());

		JLabel textLabel = new JLabel(XcosMessages.DEBUG_LEVEL_LABEL);
		debugList = new JList(DebugLevel.values());
		debugList.setSelectedIndex(diagram.getDebugLevel());
		debugList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

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
		mainFrame.add(debugList, gbc);
		
		gbc.gridwidth = 1;
		gbc.gridheight = 1;
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
				int value = ((DebugLevel) debugList.getSelectedValue()).getValue();
				diagram.setDebugLevel(value);
				try {
					XcosInterpreterManagement.synchronousScilabExec("scicos_debug(" + value + ");");
				} catch (InterpreterException e1) {
					e1.printStackTrace();
				}
		
				mainFrame.dispose();
			}
		});


		mainFrame.setMinimumSize(textLabel.getPreferredSize());
		mainFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		mainFrame.setTitle(XcosMessages.SET_DEBUG);
		mainFrame.pack();
		mainFrame.setLocationRelativeTo(null);
		mainFrame.setVisible(true);	
	}
}