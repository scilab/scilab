
/* Copyright INRIA */

package org.scilab.modules.console;

import java.awt.Point;
import java.awt.event.ActionEvent;
import java.util.List;

import com.artenum.rosetta.core.action.AbstractConsoleAction;
import com.artenum.rosetta.interfaces.core.CompletionItem;

/**
 * Class used when Scilab user asks for completion on the current edited line
 * @author Vincent COUVERT
 */
public class CompletionAction extends AbstractConsoleAction {
	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public CompletionAction() {
		super();
	}
	
	/**
	 * Threats the event
	 * @param e the action event that occured
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		Point location = configuration.getInputParsingManager().getWindowCompletionLocation();
		List<CompletionItem> completionItems = configuration.getCompletionManager().getCompletionItems();
		
		if (completionItems != null && completionItems.size() == 1) {
			/* Only one item returned, autoselected and appended to command line */
			configuration.getCompletionWindow().show(completionItems, location);
			configuration.getInputParsingManager().writeCompletionPart(configuration.getCompletionWindow().getCompletionResult());
			((SciCompletionWindow) configuration.getCompletionWindow()).setVisible(false);
		} else if (completionItems != null && completionItems.size() != 0) {
			configuration.getCompletionWindow().show(completionItems, location);
		}
	}
}
