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
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.ScrollPaneConstants;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Dialog associated with the {@link SetContextAction}.
 * 
 * Note that this dialog break the Data Abstraction Coupling metric because of
 * the numbers of graphical components involved in the GUI creation. For the
 * same reason (GUI class), constants are not used on this code.
 */
//CSOFF: ClassDataAbstractionCoupling
//CSOFF: ClassFanOutComplexity
//CSOFF: MagicNumber
public class SetContextDialog extends JDialog {
	private final ScicosParameters parameters;
	
	private JTextArea contextArea;
	
	/**
	 * Default constructor
	 * @param parent the parent component
	 * @param parameters the Scicos parameters
	 */
	public SetContextDialog(Component parent, ScicosParameters parameters) {
		this.parameters = parameters;
		
		Icon scilabIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
		Image imageForIcon = ((ImageIcon) scilabIcon).getImage();
		setLayout(new GridBagLayout());
		setIconImage(imageForIcon);
		setTitle(XcosMessages.SET_CONTEXT);
		setModal(true);
		setLocationRelativeTo(parent);
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		
		initComponents();
	}

	/**
	 * Initialize the components 
	 */
	private void initComponents() {
		JLabel textLabel = new JLabel(XcosMessages.SET_CONTEXT_LABEL_TEXT);
		
		/*
		 * Construct a text from a String array context
		 */
        StringBuilder contextBuilder = new StringBuilder();
        for (int i = 0; i < parameters.getContext().length; i++) {
        	contextBuilder.append(parameters.getContext()[i]);
        	// The '\n' is used on JTextArea for new lines.
        	contextBuilder.append("\n");
        }
        
        contextArea = new JTextArea(contextBuilder.toString());
      
        JScrollPane contextAreaScroll = new JScrollPane(contextArea, 
        		ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED, ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        
		JButton cancelButton = new JButton(XcosMessages.CANCEL);
		JButton okButton = new JButton(XcosMessages.OK);
		okButton.setPreferredSize(cancelButton.getPreferredSize());
		
        JPanel buttonPane = new JPanel();
        buttonPane.setLayout(new BoxLayout(buttonPane, BoxLayout.LINE_AXIS));
        buttonPane.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));
		buttonPane.add(okButton);
		buttonPane.add(Box.createRigidArea(new Dimension(10, 0)));
		buttonPane.add(cancelButton);
		
		/*
		 * Perform layout
		 */
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
		add(contextAreaScroll, gbc);
		


		gbc.gridheight = 1;
		gbc.gridwidth = 1;
		
		gbc.anchor = GridBagConstraints.LAST_LINE_END;
        gbc.fill = GridBagConstraints.NONE;
        gbc.weighty = 0;
		gbc.insets = new Insets(5, 0, 10, 10);
		add(buttonPane, gbc);
		
		installActionListeners(cancelButton, okButton);	
	}

	/**
	 * Install the action listener on the buttons
	 * @param cancelButton the cancel button
	 * @param okButton the OK button
	 */
	private void installActionListeners(JButton cancelButton, JButton okButton) {
		/*
		 * The cancel button just exit without doing anything
		 */
		cancelButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				dispose();
			}
		});

		/*
		 * The ok button parse the contextArea, reconstruct the real context and
		 * set the scicosParameters before exiting.
		 */
		okButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				List<String> contextList = new ArrayList<String>();
				int i = 0;
				try {
					StringReader stringReader = new StringReader(contextArea.getText());
					BufferedReader bufferReader = new BufferedReader(stringReader);
					String nextLine = "";

					while ((nextLine = bufferReader.readLine()) != null) {
						contextList.add(nextLine);
						i++;
					} 
					
					if (i == 0) {
						contextList.add("");
					}
					
					String[] context = contextList.toArray(new String[i]);
					parameters.setContext(context);
					
					// Execute the context to alert the user against wrong settings
					String ctx = contextArea.getText();
					if (!ctx.replaceAll("[^\\p{Graph}]*", "").isEmpty()) {
						// We need to remove some blanks and convert to a one line expression
						// The '\n' is used on JTextArea for new lines.
						ScilabInterpreterManagement
								.putCommandInScilabQueue(ctx
										.trim()
										.replaceAll("\n", "; ") + ";");
					}
					
					
					
					dispose();
					
				} catch (IOException e1) {
					LogFactory.getLog(SetContextAction.class).error(e1);
				} catch (PropertyVetoException e2) {
					LogFactory.getLog(SetContextAction.class).error(e2);
				}
			}
		});
	}	
}
//CSON: ClassDataAbstractionCoupling
//CSON: ClassFanOutComplexity
//CSON: MagicNumber
