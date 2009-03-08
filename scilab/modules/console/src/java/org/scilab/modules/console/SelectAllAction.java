/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
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
import javax.swing.JTextPane;
import com.artenum.rosetta.core.action.AbstractConsoleAction;

/**
 * Class used to select all in console
 * @author Allan CORNET
 */
public class SelectAllAction extends AbstractConsoleAction {
	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public SelectAllAction() {
		super();
	}

	/**
	 * Threats the event
	 * @param e the action event that occured
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		JTextPane outputPane = (JTextPane) configuration.getOutputView();
		JTextPane inputPane = (JTextPane) configuration.getInputCommandView();
		
		outputPane.setSelectionStart(0);
		outputPane.setSelectionEnd(outputPane.getText().length());
		
		inputPane.setSelectionStart(0);
		inputPane.setSelectionEnd(inputPane.getText().length());
		
	}
}
