/**
 * 
 */
package org.scilab.modules.console;

import java.awt.event.ActionEvent;
import com.artenum.rosetta.core.action.AbstractConsoleAction;

/**
 * @author Vincent COUVERT
 *
 */
public class GetNextAction extends AbstractConsoleAction {
	private static final long serialVersionUID = 1L;
	
	public void actionPerformed(ActionEvent e) {
		String historyLine = null;

		/* Search matching line in history if exists !! */
		if(configuration.getHistoryManager().isInHistory()==false) {
			/* Sets browsing state */
			configuration.getHistoryManager().setInHistory(true);
			/* Save currently edited command line */
			configuration.getHistoryManager().setTmpEntry(configuration.getInputCommandView().getText());
		}

		/* Search matching line in history if exists !! */
		historyLine = configuration.getHistoryManager().getNextEntry(configuration.getHistoryManager().getTmpEntry());

		/* If nothing found in history, go back to edited line */
		if(historyLine==null) {
			historyLine = configuration.getHistoryManager().getTmpEntry();
			/* Reset edited line */
			configuration.getHistoryManager().setTmpEntry(null);
			configuration.getHistoryManager().setInHistory(false);
		}
		
		/* If a matching entry has been found */
		if(historyLine!=null) {
			/* Update command line */
			configuration.getInputCommandView().reset();
			configuration.getInputCommandView().append(historyLine);
		}
	}
}
