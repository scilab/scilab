
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.event.ActionEvent;

import com.artenum.rosetta.core.action.AbstractConsoleAction;

/**
 * Delete the entire line when an event occurs
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class DelWholeLineAction extends AbstractConsoleAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public DelWholeLineAction() {
		super();
	}

	/**
	 * Threats the event
	 * @param e the action event that occured
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		configuration.getInputCommandView().reset();
	}

}
