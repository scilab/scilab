/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.utils;

import org.scilab.modules.action_binding.InterpreterManagement;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JMenu;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;

/**
 * Callack listener for objects in Scilab GUI
 * @author Vincent COUVERT
 */
public class SciActionListener implements ActionListener, MenuListener {

    private String sciCommand;

    /**
     * Constructor
     * @param command Scilab command to execute
     */
    public SciActionListener(String command) {
        super();
        sciCommand = command;
    }

    /**
     * What do we have to do when this action is performed ?
     * @param e the event to threat
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
        System.out.println("Action = " + sciCommand);
        InterpreterManagement.putCommandInScilabQueue(sciCommand);
    }

    /**
     * What do we have to do when this action is canceled ?
     * @param e the event to threat
     * @see javax.swing.event.MenuListener#menuCanceled(javax.swing.event.MenuEvent)
     */
    public void menuCanceled(MenuEvent e) {
        // TODO Auto-generated method stub

    }

    /**
     * What do we have to do when the menu is deselected ?
     * @param e the event to threat
     * @see javax.swing.event.MenuListener#menuCanceled(javax.swing.event.MenuEvent)
     */
    public void menuDeselected(MenuEvent e) {
        // TODO Auto-generated method stub

    }

    /**
     * What do we have to do when the menu is deselected ?
     * @param e the event to threat
     * @see javax.swing.event.MenuListener#menuCanceled(javax.swing.event.MenuEvent)
     */
    public void menuSelected(MenuEvent e) {
        // TODO Auto-generated method stub
        System.out.println("Menu Selected = " + sciCommand);
        InterpreterManagement.putCommandInScilabQueue(sciCommand);
        ((JMenu) e.getSource()).setSelected(false);
    }
}
