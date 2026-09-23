/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - Calixte DENIZET
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2021 - 2022 Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.scinotes.actions;

import java.io.File;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.filechooser.Juigetfile;
import org.scilab.modules.gui.filechooser.FileChooserInfos;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * File opening management
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 * @author Stéphane MOTTELET
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

        String[] mask = new String[] {"*.sci", "*.sce", "*.tst", "*.start", "*.quit", "*.dem","*.sci|*.sce"};

        FileChooser sfc = ScilabFileChooser.createFileChooser();
        sfc.setAcceptAllFileFilterUsed(true);
        sfc.addMask(mask, null);
        sfc.setMultipleSelection(true);
        sfc.setInitialDirectory(path);
        sfc.setTitle(SciNotesMessages.OPEN);
        sfc.setUiDialogType(Juigetfile.OPEN_DIALOG);
        sfc.displayAndWait();
        sfc.invalidate();

        String[] selection = sfc.getSelection();

        if (selection.length>0 && selection[0] != "") {
            for (int i = 0; i < selection.length; i++) {
                ConfigSciNotesManager.saveToRecentOpenedFiles(selection[i]);
                try {
                    getEditor().readFile(new File(selection[i]));
                } catch (RuntimeException ex) {
                    // Keep opening the rest of the selection if one file fails.
                    ex.printStackTrace();
                }
            }
            RecentFileAction.updateRecentOpenedFilesMenu(getEditor());
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
