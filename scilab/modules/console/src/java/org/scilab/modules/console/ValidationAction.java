package org.scilab.modules.console;

import java.awt.event.ActionEvent;

import com.artenum.rosetta.core.action.AbstractConsoleAction;
import com.artenum.rosetta.interfaces.core.InputParsingManager;
import com.artenum.rosetta.interfaces.ui.PromptView;
import com.artenum.rosetta.util.StringConstants;

/**
 * Class used to call Scilab interpreter
 * @author Vincent COUVERT
 * @author Sebastien Jourdain (jourdain@artenum.com)
 *
 */
public class ValidationAction extends AbstractConsoleAction {
	
	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public ValidationAction() {
		super();
	}

	/** 
	 * Execute a command received
	 * @param e the event to threat
	 */
	public synchronized void actionPerformed(ActionEvent e) {
		// Init
		InputParsingManager inputParsingManager = configuration.getInputParsingManager();
		PromptView promptView = configuration.getPromptView();
		String cmdToExecute = null;
		String histEntry = null;

		// Do the job
		if (inputParsingManager.isBlockEditing()) {
			// Create new line
			inputParsingManager.append(StringConstants.NEW_LINE);
			promptView.updatePrompt();
		} else {
			
			// Command to execute
			cmdToExecute = inputParsingManager.getCommandLine();
			
			// Special case: line begins with a !
			if ((cmdToExecute.length() > 0) && (cmdToExecute.charAt(0) == '!')) {
				// Cast HistoryManager to SciHistoryManager 
				// because searchBackward will not to be implemented in all not-generic console
				((SciHistoryManager) configuration.getHistoryManager()).setTmpEntry(cmdToExecute.substring(1));
				histEntry = ((SciHistoryManager) configuration.getHistoryManager()).getPreviousEntry(cmdToExecute.substring(1));
				if (histEntry != null) {
					configuration.getInputCommandView().reset();
					configuration.getInputCommandView().append(histEntry);
				}
				return;
			}
			
			// Send data to Scilab (Commands print in output view is done in sendCommandsToScilab since bug 2510 fix)
			((SciOutputView) configuration.getOutputView()).getConsole().sendCommandsToScilab(cmdToExecute, true, true);
					
		}

	}
}
