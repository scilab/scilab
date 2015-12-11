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

import java.awt.Desktop;
import java.io.File;

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Action to open a file with the default associated application
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class OpenFileWithDefaultAppAction extends CommonCallBack {

    private SwingScilabTreeTable table;

    /**
     * Default constructor
     * @param table the table associated with this action
     */
    public OpenFileWithDefaultAppAction(SwingScilabTreeTable table) {
        super(null);
        this.table = table;
    }

    /**
     * @return true if this action is available with the OS
     */
    public static boolean isSupported() {
        return Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.OPEN);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        Desktop dt = Desktop.getDesktop();
        File[] files = table.getSelectedFiles();
        for (File file : files) {
            try {
                dt.open(file);
            } catch (Exception e) {
                ScilabModalDialog.show((SwingScilabDockablePanel) SwingUtilities.getAncestorOfClass(SwingScilabDockablePanel.class, table), String.format(UiDataMessages.CANNOTOPEN, file.getName()));
            }
        }
    }
}
