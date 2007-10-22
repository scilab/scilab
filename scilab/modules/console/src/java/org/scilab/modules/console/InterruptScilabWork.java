
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.event.ActionEvent;

import javax.swing.JPanel;
import javax.swing.JTextPane;

import com.artenum.rosetta.core.action.AbstractConsoleAction;
import com.artenum.rosetta.util.StringConstants;

/**
 * Stops Scilab current work to enter pause mode
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class InterruptScilabWork extends AbstractConsoleAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public InterruptScilabWork() {
		super();
		
	}

	/**
	 * Threats the event
	 * @param e the action event that occured
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		
		if (((JTextPane) configuration.getInputCommandView()).getSelectedText() == null) {
			InterpreterManagement.interruptScilab();
		
			// If Scilab is on prompt, then emulate a user entry
			if (((JPanel) configuration.getPromptView()).isVisible()) {
				configuration.getOutputView().append(StringConstants.NEW_LINE);
				configuration.getOutputView().append(configuration.getPromptView().getDefaultPrompt());
				configuration.getOutputView().append(configuration.getInputParsingManager().getCommandLine());
				configuration.getOutputView().append(StringConstants.NEW_LINE);
				((SciInputCommandView) configuration.getInputCommandView()).setCmdBuffer("");
				configuration.getInputParsingManager().reset();
			}
		}
	}

}
