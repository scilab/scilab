/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
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
import javax.swing.JEditorPane;
import javax.swing.JTextPane;
import com.artenum.rosetta.core.action.AbstractConsoleAction;

/**
 * Class used to select all in console
 * @author Allan CORNET
 */
public class SelectAllAction extends AbstractConsoleAction {
    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public SelectAllAction() {
        super();
    }

    /**
     * Threats the event
     * @param e the action event that occurred
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
        JEditorPane outputPane = (JEditorPane) configuration.getOutputView();
        JTextPane inputPane = (JTextPane) configuration.getInputCommandView();

        outputPane.setSelectionStart(0);
        outputPane.setSelectionEnd(outputPane.getText().length());

        inputPane.setSelectionStart(0);
        inputPane.setSelectionEnd(inputPane.getText().length());

    }
}
