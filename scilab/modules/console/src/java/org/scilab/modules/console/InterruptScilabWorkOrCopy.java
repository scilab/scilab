/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

import java.awt.datatransfer.StringSelection;
import java.awt.event.ActionEvent;
import java.awt.Toolkit;
import javax.swing.JPanel;
import javax.swing.JEditorPane;
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
public class InterruptScilabWorkOrCopy extends AbstractConsoleAction {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public InterruptScilabWorkOrCopy() {
        super();

    }

    /**
     * Threats the event
     * @param e the action event that occurred
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {

        if (((JTextPane) configuration.getInputCommandView()).getSelectedText() != null) {
            /* Text selected in the input --> Copy */
            StringSelection strSelected = new StringSelection(((JTextPane) configuration.getInputCommandView()).getSelectedText());
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(strSelected, null);
        } else if (((JEditorPane) configuration.getOutputView()).getSelectedText() != null) {
            /* Text selected in the output --> Copy */
            StringSelection strSelected = new StringSelection(((JEditorPane) configuration.getOutputView()).getSelectedText());
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
