
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.event.ActionEvent;

import com.artenum.console.core.action.AbstractConsoleAction;

/**
 * Moves the caret of character to the left when an event occurs
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class NextCharAction extends AbstractConsoleAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public NextCharAction() {
		super();
	}

	/**
	 * Threats the event
	 * @param e the action event that occured
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		int currentPosition = configuration.getInputCommandView().getCaretPosition();
		
		/* Current position is not changed if already at the end of the line */
		if (currentPosition < configuration.getInputCommandViewStyledDocument().getLength()) {	
			configuration.getInputCommandView().setCaretPosition(currentPosition + 1);
		}
	}

}
