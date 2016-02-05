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

import com.artenum.rosetta.core.action.AbstractConsoleAction;
import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * Class used to open console box on windows
 * @author Allan CORNET
 */
public class ConsoleboxAction extends AbstractConsoleAction {
    private static final long serialVersionUID = 1L;
    private static final String OSNAME = System.getProperty("os.name").toLowerCase();

    /**
     * Constructor
     */
    public ConsoleboxAction() {
        super();
    }

    /**
     * check if Windows
     * @return true if windows
     */
    private static boolean isWindows() {
        return OSNAME.indexOf("windows") != -1;
    }

    /**
     * Threats the event
     * @param e the action event that occurred
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
        if (isWindows()) {
            InterpreterManagement.putCommandInScilabQueue("consolebox('toggle');");
        }
    }
}
