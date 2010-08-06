/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions.dialog;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyVetoException;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.ListSelectionModel;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.xcos.actions.DebugLevelAction;
import org.scilab.modules.xcos.actions.DebugLevelAction.DebugLevel;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Dialog for the {@link DebugLevelAction}.
 * 
 * Note that this dialog break the Data Abstraction Coupling metric because of
 * the numbers of graphical components involved in the GUI creation. For the
 * same reason (GUI class), constants are not used on this code.
 */
// CSOFF: ClassDataAbstractionCoupling
// CSOFF: MagicNumber
public class DebugLevelDialog extends JDialog {
	private final ScicosParameters parameters;
	
	private JList debugList;
	
	/**
	 * Default contructor
	 * @param parent the parent component
	 * @param parameters the associated parameters
	 */
	public DebugLevelDialog(Component parent, ScicosParameters parameters) {
		this.parameters = parameters;
		
		Icon scilabIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
		Image imageForIcon = ((ImageIcon) scilabIcon).getImage();
		setLayout(new GridBagLayout());
		setIconImage(imageForIcon);
		setTitle(XcosMessages.SET_DEBUG);
		setModal(true);
		setLocationRelativeTo(parent);
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		
		initComponents();
	}

	/**
	 * Initialize the components
	 */
	private void initComponents() {
		JLabel textLabel = new JLabel(XcosMessages.DEBUG_LEVEL_LABEL);
		debugList = new JList(DebugLevel.values());
		debugList.setSelectedIndex(parameters.getDebugLevel());
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
		add(textLabel, gbc);
		
		gbc.gridy = GridBagConstraints.RELATIVE;
		gbc.fill = GridBagConstraints.BOTH;
		gbc.weighty = 1.0;
		add(debugList, gbc);
		
		gbc.gridwidth = 1;
		gbc.gridheight = 1;
		gbc.anchor = GridBagConstraints.LAST_LINE_END;
		gbc.fill = GridBagConstraints.NONE;
		gbc.weighty = 0;
		gbc.insets = new Insets(5, 0, 10, 10);
		add(buttonPane, gbc);


		cancelButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				dispose();
			}
		});

		okButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				int value = ((DebugLevel) debugList.getSelectedValue()).getValue();
				try {
					parameters.setDebugLevel(value);
					ScilabInterpreterManagement.synchronousScilabExec("scicos_debug", value);
					dispose();
				} catch (InterpreterException e1) {
					LogFactory.getLog(DebugLevelAction.class).error(e1);
				} catch (PropertyVetoException e2) {
					LogFactory.getLog(DebugLevelAction.class).error(e2);
				}
			}
		});
	}
}
//CSON: ClassDataAbstractionCoupling
//CSON: MagicNumber
