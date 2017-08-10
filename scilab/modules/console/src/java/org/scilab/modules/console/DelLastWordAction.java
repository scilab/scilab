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
 * Delete last word and trailing spaces of the line when an event occurs
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class DelLastWordAction extends AbstractConsoleAction {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public DelLastWordAction() {
        super();
    }

    /**
     * Threats the event
     * @param e the action event that occurred
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
        int currentPosition = configuration.getInputCommandView().getCaretPosition();
        String currentText = configuration.getInputCommandView().getText();

        /* Delete traillings spaces */
        while (currentText.length() > 0 && currentText.charAt(currentText.length() - 1) == ' ') {
            currentText = currentText.substring(0, currentText.length() - 2);
        }

        /* Delete last word */
        currentText = currentText.substring(0, currentText.lastIndexOf(' ') + 1);

        configuration.getInputCommandView().setText(currentText);

        /* Sets new caret position */
        if (currentPosition <= currentText.length()) {
            configuration.getInputCommandView().setCaretPosition(currentPosition);
        } else {
            configuration.getInputCommandView().setCaretPosition(currentText.length());
        }
    }

}
