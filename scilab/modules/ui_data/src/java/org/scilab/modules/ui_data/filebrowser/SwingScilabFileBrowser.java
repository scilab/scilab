/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.ui_data.actions.HelpAction;
import org.scilab.modules.ui_data.filebrowser.actions.CloseAction;
import org.scilab.modules.ui_data.filebrowser.actions.GoToHomeAction;
import org.scilab.modules.ui_data.filebrowser.actions.GoToSCIAction;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Swing implementation of Scilab Variable browser
 * uses JTable
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class SwingScilabFileBrowser extends SwingScilabTab {

    public static final String FILEBROWSERUUID = "3b649047-6a71-4998-bd8e-00d367a4793d";

    private ScilabFileBrowserComponent filebrowser;

    /**
     * Create a JTable with data Model.
     */
    public SwingScilabFileBrowser() {
        super(UiDataMessages.FILE_BROWSER, FILEBROWSERUUID);
        setAssociatedXMLIDForHelp("filebrowser");
        addMenuBar(createMenuBar());
        filebrowser = new ScilabFileBrowserComponent();

        ToolBar toolBar = ScilabToolBar.createToolBar();
        toolBar.add(filebrowser.getPreviousButton());
        toolBar.add(filebrowser.getNextButton());
        toolBar.addSeparator();
        toolBar.add(GoToHomeAction.createButton());
        toolBar.add(GoToSCIAction.createButton());
        toolBar.addSeparator();
        toolBar.add(HelpAction.createButton(UiDataMessages.HELP));

        addToolBar(toolBar);
        setContentPane(filebrowser);
        WindowsConfigurationManager.restorationFinished(this);
    }

    /**
     * Set the base directory
     * @param baseDir the base directory
     */
    public void setBaseDir(String baseDir) {
        filebrowser.setBaseDir(baseDir);
    }

    /**
     * {@inheritDoc}
     */
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    /**
     * {@inheritDoc}
     */
    public SwingScilabWindow getParentWindow() {
        return SwingScilabWindow.allScilabWindows.get(getParentWindowId());
    }

    /**
     * {@inheritDoc}
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        setInfoBar(infoBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        setMenuBar(menuBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        setToolBar(toolBarToAdd);
    }

    /**
     * Create History Browser MenuBar
     * @return the menu bar
     */
    private static MenuBar createMenuBar() {
        MenuBar menuBar = ScilabMenuBar.createMenuBar();
        Menu fileMenu = ScilabMenu.createMenu();
        fileMenu.setText(UiDataMessages.FILE);
        fileMenu.setMnemonic('F');

        fileMenu.add(CloseAction.createMenu());

        menuBar.add(fileMenu);

        Menu helpMenu = ScilabMenu.createMenu();
        helpMenu.setText("?");
        helpMenu.setMnemonic('?');
        helpMenu.add(HelpAction.createMenuItem(UiDataMessages.HELP));
        menuBar.add(helpMenu);

        return menuBar;
    }
}
