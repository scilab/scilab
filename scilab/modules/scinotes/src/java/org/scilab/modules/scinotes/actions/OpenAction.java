/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.io.File;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.Juigetfile;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * File opening management
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public class OpenAction extends DefaultAction {

    private static final long serialVersionUID = -8765712033802048782L;
    private String initialDirectoryPath;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated SciNotes instance
     */
    public OpenAction(String name, SciNotes editor) {
        this(name, editor, "");
    }

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated SciNotes instance
     * @param path the path where to open the filechooser
     */
    public OpenAction(String name, SciNotes editor, String path) {
        super(name, editor);
        this.initialDirectoryPath = path;
    }

    /**
     * Open file action
     * @see org.scilab.modules.scinotes.actions.DefaultAction#doAction()
     */
    public void doAction() {
        String path;
        if (initialDirectoryPath.length() == 0) {
            path = getEditor().getTextPane().getName();
        } else {
            path = initialDirectoryPath;
        }

        if (path == null) {
            path = ConfigManager.getLastOpenedDirectory();
        }

        String[] mask = new String[] {"*.cos*", "*.sci", "*.sce", "*.tst", "*.start", "*.quit", "*.dem", "*.sc*", "all"};

        SwingScilabFileChooser fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());
        fileChooser.setMultiSelectionEnabled(true);
        fileChooser.setInitialDirectory(path);
        fileChooser.setAcceptAllFileFilterUsed(true);
        fileChooser.addMask(mask, null);
        fileChooser.setUiDialogType(Juigetfile.SAVE_DIALOG);

        int retval = fileChooser.showOpenDialog(getEditor());
        if (retval == JFileChooser.APPROVE_OPTION) {
            File[] f = fileChooser.getSelectedFiles();
            for (int i = 0; i < f.length; i++) {
                RecentFileAction.updateRecentOpenedFilesMenu(getEditor());
                getEditor().readFile(f[i]);
            }
        }
    }

    /**
     * Create a menu to add to SciNotes menu bar
     * @param label label of the menu
     * @param editor associated SciNotes instance
     * @param key KeyStroke
     * @return the menu
     */
    public static Object createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new OpenAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new OpenAction(tooltip, editor));
    }
}
