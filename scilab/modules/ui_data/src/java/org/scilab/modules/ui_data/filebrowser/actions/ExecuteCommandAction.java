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
