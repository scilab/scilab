/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.filebrowser.actions;

import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;

/**
 * Action to execute the file in the console
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ExecuteFileInConsoleAction extends ExecuteCommandAction {

    private static final String EXEC = "exec('%s',-1)";

    /**
     * {@inheritDoc}
     */
    public ExecuteFileInConsoleAction(SwingScilabTreeTable table) {
        super(table, EXEC);
    }
}
