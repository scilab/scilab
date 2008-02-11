
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.datatransfer.StringSelection;
import java.awt.event.ActionEvent;
import java.awt.Toolkit;
import javax.swing.JPanel;
import javax.swing.JTextPane;

import org.scilab.modules.action_binding.InterpreterManagement;

import com.artenum.rosetta.core.action.AbstractConsoleAction;
import com.artenum.rosetta.util.StringConstants;

/**
 * Stops Scilab current work to enter pause mode
 * Or copy selected text (if there is)
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
		
		if (((JTextPane) configuration.getInputCommandView()).getSelectedText() != null) {
			/* Text selected in the input --> Copy */
			StringSelection strSelected = new StringSelection(((JTextPane) configuration.getInputCommandView()).getSelectedText());
			Toolkit.getDefaultToolkit().getSystemClipboard().setContents(strSelected, null);
		} else if (((JTextPane) configuration.getOutputView()).getSelectedText() != null) {
			/* Text selected in the output --> Copy */
			StringSelection strSelected = new StringSelection(((JTextPane) configuration.getOutputView()).getSelectedText());
			Toolkit.getDefaultToolkit().getSystemClipboard().setContents(strSelected, null);
		} else {
			/* Interrupt Scilab */
			InterpreterManagement.interruptScilab();
		
			// If Scilab is on prompt, then emulate a user entry
			if (((JPanel) configuration.getPromptView()).isVisible()) {
				configuration.getOutputView().append(StringConstants.NEW_LINE);
				configuration.getOutputView().append(configuration.getPromptView().getDefaultPrompt());
				configuration.getOutputView().append(configuration.getInputParsingManager().getCommandLine());
				configuration.getOutputView().append(StringConstants.NEW_LINE);
				((SciOutputView) configuration.getOutputView()).getConsole().sendCommandsToScilab("", false, false);
				configuration.getInputParsingManager().reset();
			}
		}
	}

}
