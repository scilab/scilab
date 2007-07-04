package org.scilab.modules.console;

import java.awt.event.ActionEvent;

import com.artenum.console.core.action.AbstractConsoleAction;
import com.artenum.console.interfaces.core.InputParsingManager;
import com.artenum.console.interfaces.ui.OutputView;
import com.artenum.console.interfaces.ui.PromptView;
import com.artenum.console.util.StringConstants;

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
				histEntry = ((SciHistoryManager) configuration.getHistoryManager()).searchBackward(cmdToExecute.substring(1));
				if (histEntry != null) {
					configuration.getInputCommandView().reset();
					configuration.getInputCommandView().append(histEntry);
				}
				return;
			}
			
			// Reset command line
			inputParsingManager.reset();
			promptView.updatePrompt();
			
			// Reset history settings
			configuration.getHistoryManager().setTmpEntry(null);
			configuration.getHistoryManager().setInHistory(false);
			
			// Hide the prompt and command line
			configuration.getInputCommandView().setVisible(false);
			configuration.getPromptView().setVisible(false);

			// Print the command in the output view
			boolean firstPrompt = true;
			for (String line : cmdToExecute.split(StringConstants.NEW_LINE)) {
				
				outputView.append(StringConstants.NEW_LINE);
				if (firstPrompt) {
					firstPrompt = false;
					outputView.append(promptView.getDefaultPrompt());
				} else {
					outputView.append(promptView.getInBlockPrompt());
				}
				outputView.append(line);
			}
			outputView.append(StringConstants.NEW_LINE);
			
			// Store the command in the buffer so that Scilab can read it
			((SciInputCommandView) configuration.getInputCommandView()).setCmdBuffer(cmdToExecute);
			((SciHistoryManager) configuration.getHistoryManager()).addEntry(cmdToExecute);
		}

	}
}
