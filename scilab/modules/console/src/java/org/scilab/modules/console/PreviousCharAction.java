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

import java.awt.event.ActionEvent;

import com.artenum.rosetta.core.action.AbstractConsoleAction;

/**
 * Moves the caret of character to the right when an event occurs
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class PreviousCharAction extends AbstractConsoleAction {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public PreviousCharAction() {
        super();
    }

    /**
     * Threats the event
     * @param e the action event that occurred
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
        int currentPosition = configuration.getInputCommandView().getCaretPosition();
        /* Current position is not changed if already at the beginning of the line */
        if (currentPosition > 0) {
            configuration.getInputCommandView().setCaretPosition(currentPosition - 1);
        }
    }

}
