
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.event.ActionEvent;

import javax.swing.text.BadLocationException;

import com.artenum.console.core.action.AbstractConsoleAction;

/**
 * Delete the line from the caret postion to the end of the line when an event occurs
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class DelEndOfLineAction extends AbstractConsoleAction {

	private static final long serialVersionUID = 1L;
	
	/**
	 * Constructor
	 */
	public DelEndOfLineAction() {
		super();
	}

	/**
	 * Threats the event
	 * @param e the action event that occured
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		int currentPosition = configuration.getInputCommandView().getCaretPosition();
		int textLength = configuration.getInputCommandViewStyledDocument().getLength();
		
		try {
			configuration.getInputCommandViewStyledDocument().remove(currentPosition, textLength - currentPosition);
		} catch (BadLocationException e1) {
			e1.printStackTrace();
		}
	}

}
