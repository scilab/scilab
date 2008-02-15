/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.console;

import java.awt.event.ActionEvent;

import com.artenum.rosetta.core.action.AbstractConsoleAction;

/**
 * Delete last word and trailing spaces of the line when an event occurs
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class DelLastWordAction extends AbstractConsoleAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public DelLastWordAction() {
		super();
	}

	/**
	 * Threats the event
	 * @param e the action event that occured
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		int currentPosition = configuration.getInputCommandView().getCaretPosition();
		String currentText = configuration.getInputCommandView().getText();

		/* Delete traillings spaces */
		while (currentText.length() > 0 && currentText.charAt(currentText.length() - 1) == ' ') {
			currentText = currentText.substring(0, currentText.length() - 2);
		}

		/* Delete last word */
		currentText = currentText.substring(0, currentText.lastIndexOf(' ') + 1);

		configuration.getInputCommandView().setText(currentText);

		/* Sets new caret position */
		if (currentPosition <= currentText.length()) {
			configuration.getInputCommandView().setCaretPosition(currentPosition);
		} else {
			configuration.getInputCommandView().setCaretPosition(currentText.length());
		}
	}

}
