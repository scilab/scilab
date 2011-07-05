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

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.filebrowser.actions.CloseAction;
import org.scilab.modules.ui_data.filebrowser.actions.GoToHomeAction;
import org.scilab.modules.ui_data.filebrowser.actions.GoToSCIAction;

/**
 * Swing implementation of Scilab Variable browser
 * uses JTable
 * @author Calixte DENIZET
 */
public final class SwingScilabFileBrowser extends SwingScilabTab implements Tab {

    public static final String FILEBROWSERUUID = "3b649047-6a71-4998-bd8e-00d367a4793d";

    private ScilabFileBrowserComponent filebrowser;

    /**
     * Create a JTable with data Model.
     * @param columnsName : Titles of JTable columns.
     */
    public SwingScilabFileBrowser() {
        super(UiDataMessages.FILE_BROWSER, FILEBROWSERUUID);
        addMenuBar(createMenuBar());
        filebrowser = new ScilabFileBrowserComponent();

        ToolBar toolBar = ScilabToolBar.createToolBar();
        toolBar.add(filebrowser.getPreviousButton());
        toolBar.add(filebrowser.getNextButton());
        toolBar.addSeparator();
        toolBar.add(GoToHomeAction.createButton());
        toolBar.add(GoToSCIAction.createButton());

        addToolBar(toolBar);
        setContentPane(filebrowser);
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
    public Window getParentWindow() {
        return (Window) UIElementMapper.getCorrespondingUIElement(getParentWindowId());
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

        return menuBar;
    }
}
