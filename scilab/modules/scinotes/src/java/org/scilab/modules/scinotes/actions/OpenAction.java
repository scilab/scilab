/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.Juigetfile;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * File opening management
 * @author Bruno JOFRET
 */
public final class OpenAction extends DefaultAction {

    private static final long serialVersionUID = -8765712033802048782L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated SciNotes instance
     */
    public OpenAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Open file action
     * @see org.scilab.modules.scinotes.actions.DefaultAction#doAction()
     */
    public void doAction() {

        String initialDirectoryPath = getEditor().getTextPane().getName();
        if (initialDirectoryPath == null) {
            initialDirectoryPath = ConfigManager.getLastOpenedDirectory();
        }

        String[] mask = new String[]{"*.cos*", "*.sci", "*.sce", "*.tst", "*.start", "*.quit", "*.sc*", "all"};

        SwingScilabFileChooser fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());
        fileChooser.setInitialDirectory(initialDirectoryPath);
        fileChooser.setAcceptAllFileFilterUsed(true);
        fileChooser.addMask(mask, null);
        fileChooser.setUiDialogType(Juigetfile.SAVE_DIALOG);

        int retval = fileChooser.showOpenDialog(getEditor());
        if (retval == JFileChooser.APPROVE_OPTION) {
            File f = fileChooser.getSelectedFile();
            ConfigManager.saveLastOpenedDirectory(f.getPath());
            ConfigSciNotesManager.saveToRecentOpenedFiles(f.getPath());

            getEditor().setTitle(f.getPath() + " - " + SciNotesMessages.SCILAB_EDITOR);
            RecentFileAction.updateRecentOpenedFilesMenu(getEditor());
            getEditor().readFile(f);
        }
    }

    /**
     * Create a menu to add to SciNotes menu bar
     * @param label label of the menu
     * @param editor associated SciNotes instance
     * @param key KeyStroke
     * @return the menu
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new OpenAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new OpenAction(tooltip, editor));
    }
}
