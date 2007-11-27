
/* Copyright INRIA 2007 */

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

	public void menuCanceled(MenuEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void menuDeselected(MenuEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void menuSelected(MenuEvent e) {
		// TODO Auto-generated method stub
		System.out.println("Menu Selected = " + sciCommand);
		InterpreterManagement.putCommandInScilabQueue(sciCommand);
		((JMenu) e.getSource()).setSelected(false);
	}
}
