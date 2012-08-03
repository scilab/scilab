/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
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

/**
 * Moves the caret of character to the left when an event occurs
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class NextCharAction extends AbstractConsoleAction {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public NextCharAction() {
        super();
    }

    /**
     * Threats the event
     * @param e the action event that occurred
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
        int currentPosition = configuration.getInputCommandView().getCaretPosition();

        /* Current position is not changed if already at the end of the line */
        if (currentPosition < configuration.getInputCommandViewStyledDocument().getLength()) {
            configuration.getInputCommandView().setCaretPosition(currentPosition + 1);
        }
    }

}
