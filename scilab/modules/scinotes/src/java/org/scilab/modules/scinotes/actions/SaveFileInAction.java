/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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
import java.util.List;

import javax.swing.KeyStroke;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * Action to save in a predefined directory
 * @author Calixte DENIZET
 */
public final class SaveFileInAction extends SaveAsAction {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     * @param name the name of the action
     * @param path the path where to save
     * @param editor associated SciNotes instance
     */
    public SaveFileInAction(String name, String path, SciNotes editor) {
        super(name, path, editor);
    }

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated SciNotes instance
     */
    public SaveFileInAction(String name, SciNotes editor) {
        super(name, null, editor);
    }

    /**
     * Create a menu to add to SciNotes menu bar
     * @param label label of the menu
     * @param editor associated SciNotes instance
     * @param key KeyStroke
     * @return the menu
     */
    public static Object createMenu(String label, final SciNotes editor, KeyStroke key) {
        final Menu favoriteDirs = ScilabMenu.createMenu();
        ((SwingScilabMenu) favoriteDirs.getAsSimpleMenu()).addMenuListener(new MenuListener() {

            public void menuCanceled(MenuEvent e) { }
            public void menuDeselected(MenuEvent e) { }
            public void menuSelected(MenuEvent e) {
                updateFavoriteDirsMenu(favoriteDirs, editor);
            }
        });

        favoriteDirs.setText(label);
        updateFavoriteDirsMenu(favoriteDirs, editor);
        return favoriteDirs;
    }

    /**
     * Create a menu to add to SciNotes menu bar
     * @param editor associated SciNotes instance
     * @param path the path to open
     * @return the menuitem
     */
    public static MenuItem createMenu(SciNotes editor, String path) {
        return createMenu(path, null, new SaveFileInAction(path, path, editor), null);
    }

    /**
     * Update menu displaying favorite directories.
     * @param menu the menu to update
     * @param editor SciNotes
     */
    public static void updateFavoriteDirsMenu(Menu menu, SciNotes editor) {
        ((SwingScilabMenu) menu.getAsSimpleMenu()).removeAll();
        List<File> dirs = ConfigSciNotesManager.getAllFavoriteDirs();

        for (int i = 0; i < dirs.size(); i++) {
            menu.add(createMenu(editor, dirs.get(i).getPath()));
        }
    }
}
