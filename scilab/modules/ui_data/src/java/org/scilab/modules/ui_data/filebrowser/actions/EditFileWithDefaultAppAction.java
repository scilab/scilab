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

import java.awt.Desktop;
import java.io.File;

import javax.swing.SwingUtilities;

import org.scilab.modules.commons.OS;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Action to edit a file with the default associated application
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class EditFileWithDefaultAppAction extends CommonCallBack {

    private static final boolean isWindows = (OS.get() == OS.WINDOWS);

    private SwingScilabTreeTable table;

    /**
     * Default constructor
     * @param table the table associated with this action
     */
    public EditFileWithDefaultAppAction(SwingScilabTreeTable table) {
        super(null);
        this.table = table;
    }

    /**
     * @return true if this action is available with the OS
     */
    public static boolean isSupported() {
        return Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.EDIT);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        Desktop dt = Desktop.getDesktop();
        File[] files = table.getSelectedFiles();
        for (File file : files) {
            try {
                if (isWindows) {
                    String cmd = "rundll32 url.dll,FileProtocolHandler " + file.getCanonicalPath();
                    Runtime.getRuntime().exec(cmd);
                } else {
                    dt.edit(file);
                }
            } catch (Exception e) {
                ScilabModalDialog.show((SwingScilabDockablePanel) SwingUtilities.getAncestorOfClass(SwingScilabDockablePanel.class, table), String.format(UiDataMessages.CANNOTREAD, file.getName()));
            }
        }
    }
}
