/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
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
