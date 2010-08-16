/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * Action to open the filechooser in a predefined directory
 * @author Calixte DENIZET
 */
public final class OpenFileInAction extends OpenAction {

    private static final long serialVersionUID = 1L;
    private static final String SCI = "SCI";
    private static final String SEP = ";";

    private static MenuItem addDir;
    private static MenuItem rmDir;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated SciNotes instance
     */
    public OpenFileInAction(String name, SciNotes editor) {
        super(name, editor, name);
    }

    /**
     * Create a menu to add to SciNotes menu bar
     * @param label label of the menu
     * @param editor associated SciNotes instance
     * @param key KeyStroke
     * @return the menu
     */
    public static Object createMenu(String label, final SciNotes editor, KeyStroke key) {
        StringTokenizer token = new StringTokenizer(label, SEP);
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
        return createMenu(path, null, new OpenFileInAction(path, editor), null);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(String tooltip, String icon, SciNotes editor) {
        StringTokenizer token = new StringTokenizer(tooltip, SEP);
        String tt = token.nextToken();
        String path = token.nextToken();
        if (path.equals(SCI)) {
            path = System.getenv(SCI);
        } else if (path.equals("MODULES")) {
            path = System.getenv(SCI) + "/modules";
        }
        return createButton(tt + path, icon, new OpenFileInAction(path, editor));
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
