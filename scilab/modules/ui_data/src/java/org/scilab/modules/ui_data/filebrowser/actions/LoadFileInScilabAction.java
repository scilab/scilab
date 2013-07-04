/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser.actions;

import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;

/**
 * Action to load the file in Scilab
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class LoadFileInScilabAction extends ExecuteCommandAction {

    private static final String LOAD = "load('%s')";

    /**
     * {@inheritDoc}
     */
    public LoadFileInScilabAction(SwingScilabTreeTable table) {
        super(table, LOAD);
    }
}
