/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
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
