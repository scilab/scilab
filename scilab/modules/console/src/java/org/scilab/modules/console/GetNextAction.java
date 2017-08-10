/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
        if (configuration.getHistoryManager().isInHistory() == false) {
            /* Sets browsing state */
            configuration.getHistoryManager().setInHistory(true);
            /* Save currently edited command line */
            configuration.getHistoryManager().setTmpEntry(configuration.getInputCommandView().getText());
        }

        /* Search matching line in history if exists !! */
        historyLine = configuration.getHistoryManager().getNextEntry(configuration.getHistoryManager().getTmpEntry());

        /* If nothing found in history, go back to edited line */
        if (historyLine == null) {
            historyLine = configuration.getHistoryManager().getTmpEntry();
            /* Reset edited line */
            configuration.getHistoryManager().setTmpEntry(null);
            configuration.getHistoryManager().setInHistory(false);
        }

        /* If a matching entry has been found */
        if (historyLine != null) {
            /* Update command line */
            configuration.getInputCommandView().reset();
            configuration.getInputCommandView().append(historyLine);
        }
    }
}
