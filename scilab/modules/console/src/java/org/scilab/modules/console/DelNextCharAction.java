/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
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

import javax.swing.text.BadLocationException;

import com.artenum.rosetta.core.action.AbstractConsoleAction;

/**
 * Delete character following the caret when an event occurs (equivalent of a SUPPR key press)
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class DelNextCharAction extends AbstractConsoleAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public DelNextCharAction() {
		super();
	}

	/**
	 * Threats the event
	 * @param e the action event that occured
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		int currentPosition = configuration.getInputCommandView().getCaretPosition();

		/* Do not try to remove an non-existing item */
		if (currentPosition < configuration.getInputCommandViewStyledDocument().getLength()) {
			try {
				configuration.getInputCommandViewStyledDocument().remove(currentPosition, 1);
			} catch (BadLocationException e1) {
				e1.printStackTrace();
			}
		}
	}
}
