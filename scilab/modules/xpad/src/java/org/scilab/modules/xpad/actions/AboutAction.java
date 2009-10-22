/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad.actions;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

public class AboutAction extends DefaultAction {

	private static boolean windowAlreadyExist = false;
	private static JFrame mainFrame;
	
    private AboutAction(Xpad editor) {
	super(XpadMessages.ABOUT, editor);
    }

    public static MenuItem createMenu(Xpad editor) {
	return createMenu(XpadMessages.ABOUT, null, new AboutAction(editor), null);
    }
    
	public void doAction() {
    	if (!AboutAction.windowAlreadyExist) {
    		aboutBox();
    		AboutAction.windowAlreadyExist = true;
    	}
	}
    	
    public void aboutBox(){
        mainFrame = new JFrame();
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainFrame.setLayout(new GridBagLayout());


        JLabel aboutLabel  = new JLabel(XpadMessages.XPAD_VERSION);
        
        JButton okButton = new JButton(XpadMessages.OK);

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = 0;
        gbc.gridx = 0;
        gbc.gridheight = 1;
        gbc.gridwidth = 1;
        gbc.insets = new Insets(0, 10, 0, 0);




        gbc.gridx = 0;
        gbc.gridy = 4;
        gbc.gridheight = 1;
        gbc.gridwidth = 1;
        gbc.fill = GridBagConstraints.NONE;
        gbc.insets = new Insets(0, 10, 0, 0);
        mainFrame.add( aboutLabel, gbc);


        gbc.gridx = 1;
        gbc.gridy = 5;
        gbc.gridheight = 1;
        gbc.gridwidth = 1;
        gbc.weightx = 1.;
        gbc.fill = GridBagConstraints.NONE;

        gbc.gridx = 2;
        gbc.weightx = 0.;
        gbc.insets = new Insets(5, 0, 10, 10);
        mainFrame.add(okButton, gbc);

	
        
		okButton.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				AboutAction.windowAlreadyExist = false;
				mainFrame.dispose();
			}
		});
		
		
        
	//display the frame and set some properties
		
		mainFrame.addWindowListener(new WindowListener() {
			public void windowClosed(WindowEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			public void windowDeiconified(WindowEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			public void windowActivated(WindowEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			public void windowClosing(WindowEvent arg0) {
				AboutAction.windowAlreadyExist = false;
				mainFrame.dispose();
				
			}
			public void windowDeactivated(WindowEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			public void windowIconified(WindowEvent arg0) {
				
			};
			public void windowOpened(WindowEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			
		});
		
		
        mainFrame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        mainFrame.setTitle(XpadMessages.ABOUT);
        mainFrame.setSize(250, 80);
        //mainFrame.pack();
        mainFrame.setLocationRelativeTo(null);
        mainFrame.setVisible(true);		
    	
    }
}
