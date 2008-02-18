/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import org.scilab.modules.action_binding.InterpreterManagement;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JMenu;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;

/**
 * Callack listener for objects in Scilab GUI
 * @author Vincent COUVERT
 */
public class SciActionListener implements ActionListener, MenuListener {

	private String sciCommand;
	
	/**
	 * Constructor
	 * @param command Scilab command to execute
	 */
	public SciActionListener(String command) {
		super();
		sciCommand = command;
	}

	/**
	 * What do we have to do when this action is performed ?
	 * @param e the event to threat
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		System.out.println("Action = " + sciCommand);
		InterpreterManagement.putCommandInScilabQueue(sciCommand);
	}

	/**
	 * What do we have to do when this action is canceled ?
	 * @param e the event to threat
	 * @see javax.swing.event.MenuListener#menuCanceled(javax.swing.event.MenuEvent)
	 */
	public void menuCanceled(MenuEvent e) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * What do we have to do when the menu is deselected ?
	 * @param e the event to threat
	 * @see javax.swing.event.MenuListener#menuCanceled(javax.swing.event.MenuEvent)
	 */
	public void menuDeselected(MenuEvent e) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * What do we have to do when the menu is deselected ?
	 * @param e the event to threat
	 * @see javax.swing.event.MenuListener#menuCanceled(javax.swing.event.MenuEvent)
	 */
	public void menuSelected(MenuEvent e) {
		// TODO Auto-generated method stub
		System.out.println("Menu Selected = " + sciCommand);
		InterpreterManagement.putCommandInScilabQueue(sciCommand);
		((JMenu) e.getSource()).setSelected(false);
	}
}
