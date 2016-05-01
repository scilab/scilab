/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
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

import javax.swing.text.BadLocationException;

import com.artenum.rosetta.core.action.AbstractConsoleAction;
import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * Delete character following the caret when an event occurs (equivalent of a SUPPR key press)
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class DelNextCharAction extends AbstractConsoleAction {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public DelNextCharAction() {
        super();
    }

    /**
     * Threats the event
     * @param e the action event that occurred
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
        int len = configuration.getInputCommandViewStyledDocument().getLength();
        /*CTRL + D on empty line, send exit command to Scilab*/
        if (len == 0) {
            InterpreterManagement.requestScilabExec("exit");
            return;
        }
        int currentPosition = configuration.getInputCommandView().getCaretPosition();

        /* Do not try to remove an non-existing item */
        if (currentPosition < len) {
            try {
                configuration.getInputCommandViewStyledDocument().remove(currentPosition, 1);
            } catch (BadLocationException e1) {
                e1.printStackTrace();
            }
        }
    }
}
