/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Allan SIMON
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
package org.scilab.modules.scinotes.actions;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.SciNotesOptions;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;


/**
 * RecentFileAction Class
 * @author Allan SIMON
 *
 */
@SuppressWarnings(value = { "serial" })
public class RecentFileAction extends DefaultAction {

    private static Map<SciNotes, Menu> menus = new HashMap<SciNotes, Menu>();

    private File recentFile;

    /**
     * Constructor
     * @param editor SciNotes
     * @param f File
     */
    public RecentFileAction(SciNotes editor , File f) {
        super(f.getName(), editor);
        recentFile = f;
    }

    /**
     * doAction
     */
    public void doAction() {
        ConfigSciNotesManager.saveToRecentOpenedFiles(recentFile.getPath());
        getEditor().readFile(recentFile);
        getEditor().setTitle(recentFile.getPath() + " - SciNotes");
    }

    /**
     * Clean
     */
    public static void close(SciNotes ed) {
        menus.remove(ed);
    }

    /**
     * createMenu
     * @param editor SciNotes
     * @param f File
     * @return a MenuItem
     */
    public static MenuItem createMenu(SciNotes editor, File f) {
        return createMenu(f.getName(), null, new RecentFileAction(editor, f), null);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor associated editor
     * @param key KeyStroke
     * @return the menu
     */
    public static Menu createMenu(String label, SciNotes editor, KeyStroke key) {
        Menu recentsMenu = ScilabMenu.createMenu();
        recentsMenu.setText(label);
        menus.put(editor, recentsMenu);

        updateRecentOpenedFilesMenu(editor);
        return recentsMenu;
    }

    /**
     * Update menu displaying recent opened files.
     * @param editor SciNotes
     */
    public static void updateRecentOpenedFilesMenu() {
        List<File> recentFiles = ConfigSciNotesManager.getAllRecentOpenedFiles();
        for (SciNotes ed : menus.keySet()) {
            Menu recentsMenu = menus.get(ed);
            if (recentsMenu != null) {
                ((SwingScilabMenu) recentsMenu.getAsSimpleMenu()).removeAll();
                final int N = Math.min(recentFiles.size(), SciNotesOptions.getSciNotesPreferences().numberOfRecentlyOpen);
                for (int i = N - 1; i >= 0; i--) {
                    recentsMenu.add(RecentFileAction.createMenu(ed, recentFiles.get(i)));
                }
            }
        }
    }

    /**
     * Update menu displaying recent opened files.
     * @param editor SciNotes
     */
    public static void updateRecentOpenedFilesMenu(SciNotes editor) {
        List<File> recentFiles = ConfigSciNotesManager.getAllRecentOpenedFiles();
        Menu recentsMenu = menus.get(editor);
        if (recentsMenu != null) {
            ((SwingScilabMenu) recentsMenu.getAsSimpleMenu()).removeAll();
            final int N = Math.min(recentFiles.size(), SciNotesOptions.getSciNotesPreferences().numberOfRecentlyOpen);
            for (int i = N - 1; i >= 0; i--) {
                recentsMenu.add(RecentFileAction.createMenu(editor, recentFiles.get(i)));
            }
        }
    }
}
