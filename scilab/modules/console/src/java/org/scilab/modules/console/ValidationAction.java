package org.scilab.modules.console;

import java.awt.event.ActionEvent;

import com.artenum.rosetta.core.action.AbstractConsoleAction;
import com.artenum.rosetta.interfaces.core.InputParsingManager;
import com.artenum.rosetta.interfaces.ui.OutputView;
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
		OutputView outputView = configuration.getOutputView();
		PromptView promptView = configuration.getPromptView();
		String cmdToExecute = null;
		String histEntry = null;
		
		outputView.setCaretPositionToEnd();

		// Do the job
		if (inputParsingManager.isBlockEditing()) {
			// Create new line
			inputParsingManager.append(StringConstants.NEW_LINE);
			promptView.updatePrompt();
		} else {
			
			// Command to execute
			cmdToExecute = inputParsingManager.getCommandLine();
			
			// Special case: line begins with a !
			if (!cmdToExecute.isEmpty() && cmdToExecute.charAt(0) == '!') {
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
			
			// Print the command in the output view
			boolean firstPrompt = true;
			outputView.setCaretPositionToEnd();
			for (String line : cmdToExecute.split(StringConstants.NEW_LINE)) {
				if (firstPrompt) {
					firstPrompt = false;
					outputView.append(StringConstants.NEW_LINE);
					outputView.append(promptView.getDefaultPrompt());
				}
				outputView.append(line);
				outputView.append(StringConstants.NEW_LINE);
			}
			
			// Send data to Scilab
			((SciOutputView) configuration.getOutputView()).getConsole().sendCommandsToScilab(cmdToExecute, false);
					
		}

	}
}
