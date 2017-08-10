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
import java.util.StringTokenizer;

import javax.swing.JFileChooser;
import javax.swing.KeyStroke;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;

import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * Action to register favorite directories
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class RegisterFavoriteDirsAction extends DefaultAction {

    private static MenuItem addDir;
    private static MenuItem rmDir;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated SciNotes instance
     */
    public RegisterFavoriteDirsAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Clean
     */
    public static void close() {
        addDir = null;
        rmDir = null;
    }

    /**
     * Create a menu to add to SciNotes menu bar
     * @param label label of the menu
     * @param editor associated SciNotes instance
     * @param key KeyStroke
     * @return the menu
     */
    public static Object createMenu(String label, final SciNotes editor, KeyStroke key) {
        StringTokenizer token = new StringTokenizer(label, ";\uff1b");
        String label1 = token.nextToken();
        String label2 = token.nextToken();
        String label3 = token.nextToken();

        final Menu favoriteDirs = ScilabMenu.createMenu();
        ((SwingScilabMenu) favoriteDirs.getAsSimpleMenu()).addMenuListener(new MenuListener() {

            public void menuCanceled(MenuEvent e) { }
            public void menuDeselected(MenuEvent e) { }
            public void menuSelected(MenuEvent e) {
                updateFavoriteDirsMenu(favoriteDirs, editor);
            }
        });

        favoriteDirs.setText(label1);
        addDir = AddDirectoryAction.createMenu(favoriteDirs, label2, editor, null);
        rmDir = RmDirectoryAction.createMenu(favoriteDirs, label3, editor, null);
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
        return createMenu(path, null, new DefaultAction("", editor) {
            public void doAction() { }
        }, null);
    }

    /**
     * Update menu displaying favorite directories.
     * @param menu the menu to update
     * @param editor SciNotes
     */
    public static void updateFavoriteDirsMenu(Menu menu, SciNotes editor) {
        ((SwingScilabMenu) menu.getAsSimpleMenu()).removeAll();
        menu.add(addDir);
        menu.add(rmDir);

        List<File> dirs = ConfigSciNotesManager.getAllFavoriteDirs();

        if (dirs.size() != 0) {
            menu.addSeparator();
        }

        for (int i = 0; i < dirs.size(); i++) {
            menu.add(createMenu(editor, dirs.get(i).getPath()));
        }
    }

    /**
     * Inner class to handle the action which consists to add a new directory
     */
    @SuppressWarnings(value = { "serial" })
    static class AddDirectoryAction extends DefaultAction {

        private Menu menu;

        /**
         * Constructor
         * @param menu the menu associated with this action
         * @param name the name of the action
         * @param editor associated SciNotes instance
         */
        public AddDirectoryAction(Menu menu, String name, SciNotes editor) {
            super(name, editor);
            this.menu = menu;
        }

        /**
         * Open file action
         * @see org.scilab.modules.scinotes.actions.DefaultAction#doAction()
         */
        public void doAction() {
            SwingScilabFileChooser fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());
            fileChooser.setDialogTitle(getCommand());
            fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
            fileChooser.setAcceptAllFileFilterUsed(false);
            if (fileChooser.showOpenDialog(getEditor()) == JFileChooser.APPROVE_OPTION) {
                File path = fileChooser.getSelectedFile();
                if (path == null || path.isFile()) {
                    path = fileChooser.getCurrentDirectory();
                }
                ConfigSciNotesManager.saveFavoriteDirectory(path.getPath());
                updateFavoriteDirsMenu(menu, getEditor());
            }
        }

        /**
         * Create a menu to add to SciNotes menu bar
         * @param menu the menu associated with this action
         * @param label label of the menu
         * @param editor associated SciNotes instance
         * @param key KeyStroke
         * @return the menu
         */
        public static MenuItem createMenu(Menu menu, String label, SciNotes editor, KeyStroke key) {
            return createMenu(label, null, new AddDirectoryAction(menu, label, editor), key);
        }
    }

    /**
     * Inner class to handle the action which consists to remove a directory
    @SuppressWarnings(value = { "serial" })
     */
    static class RmDirectoryAction extends DefaultAction {

        private Menu menu;

        /**
         * Constructor
         * @param menu the menu associated with this action
         * @param name the name of the action
         * @param editor associated SciNotes instance
         */
        public RmDirectoryAction(Menu menu, String name, SciNotes editor) {
            super(name, editor);
            this.menu = menu;
        }

        /**
         * Open file action
         * @see org.scilab.modules.scinotes.actions.DefaultAction#doAction()
         */
        public void doAction() {
            ConfigSciNotesManager.rmLastFavoriteDirectory();
            updateFavoriteDirsMenu(menu, getEditor());
        }

        /**
         * Create a menu to add to SciNotes menu bar
         * @param menu the menu associated with this action
         * @param label label of the menu
         * @param editor associated SciNotes instance
         * @param key KeyStroke
         * @return the menu
         */
        public static MenuItem createMenu(Menu menu, String label, SciNotes editor, KeyStroke key) {
            return createMenu(label, null, new RmDirectoryAction(menu, label, editor), key);
        }
    }
}
