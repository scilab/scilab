/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - Artenum - Sebastien JOURDAIN
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
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

import javax.swing.JTextPane;

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

        /* If occurred during a "more" message */
        if (!((JTextPane) configuration.getInputCommandView()).isEditable()) {
            return;
        }

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
