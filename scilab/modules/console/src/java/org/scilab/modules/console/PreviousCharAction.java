
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.event.ActionEvent;

import com.artenum.rosetta.core.action.AbstractConsoleAction;

/**
 * Moves the caret of character to the right when an event occurs
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class PreviousCharAction extends AbstractConsoleAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public PreviousCharAction() {
		super();
	}

	/**
	 * Threats the event
	 * @param e the action event that occured
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		int currentPosition = configuration.getInputCommandView().getCaretPosition();
		/* Current position is not changed if already at the beginning of the line */
		if (currentPosition > 0) {	
			configuration.getInputCommandView().setCaretPosition(currentPosition - 1);
		}
	}

}
