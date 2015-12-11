/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser.actions;

import java.io.File;

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;

/**
 * Action to execute a command
 * @author Calixte DENIZET
 */

@SuppressWarnings(value = { "serial" })
public class ExecuteCommandAction extends CommonCallBack {

    private SwingScilabTreeTable table;
    private String cmd;

    /**
     * Default constructor
     * @param table the table associated with this action
     * @param the command with a %s which will be replaced by the file name
     */
    public ExecuteCommandAction(SwingScilabTreeTable table, String cmd) {
        super(null);
        this.table = table;
        this.cmd = cmd;
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        String[] paths = table.getSelectedPaths();
        for (String path : paths) {
            File f = new File(path);
            if (f.exists() && f.canRead() && f.isFile()) {
                try {
                    String com = String.format(cmd, path);
                    ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(com, true /* display */, true /* store in history */);
                } catch (NoClassDefFoundError e) {
                    ScilabModalDialog.show((SwingScilabDockablePanel) SwingUtilities.getAncestorOfClass(SwingScilabDockablePanel.class, table), Messages.gettext("Feature not available in this mode..."));
                }
            }
        }
    }
}
