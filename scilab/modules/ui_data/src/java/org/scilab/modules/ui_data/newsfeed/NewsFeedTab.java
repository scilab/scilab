/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
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

package org.scilab.modules.ui_data.newsfeed;

import org.scilab.modules.ui_data.newsfeed.actions.CloseAction;
import org.scilab.modules.ui_data.newsfeed.actions.HelpAction;
import org.scilab.modules.ui_data.newsfeed.actions.UpdateAction;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;

import org.scilab.modules.ui_data.utils.UiDataMessages;

import java.util.List;
import java.util.ListIterator;


/**
 * Main class to display a news feed in a tab Dockable panel (jflexdock) For
 * now, manages one tab only
 */
public class NewsFeedTab extends SwingScilabDockablePanel implements SimpleTab {

    public static final String NEWSFEED_UUID = "DC0957B3-81DA-4E39-B0B5-E93B35412162";
    public static final String NEWSFEED_HELP_ID = "newsfeed";

    private static NewsFeedTab instance = null;

    private NewsFeedWidget newsFeedWidget;
    private NewsFeedController newsFeedController;

    static {
        ScilabTabFactory.getInstance().addTabFactory(NewsFeedTabFactory.getInstance());
    }

    public static NewsFeedTab getInstance() {
        if (instance == null) {
            instance = new NewsFeedTab();
        }
        return instance;
    }

    private NewsFeedTab() {
        super(NewsFeedUIMessages.NEWS_FEED, NEWSFEED_UUID);

        newsFeedController = new NewsFeedController();
        newsFeedWidget = new NewsFeedWidget(newsFeedController);
        newsFeedController.addNewsFeedEventListener(newsFeedWidget);

        setContentPane(newsFeedWidget);

        addMenuBar(createMenuBar());
        addToolBar(createToolBar());
        TextBox infobar = ScilabTextBox.createTextBox();
        addInfoBar(infobar);

        registerClosingOperation();
        WindowsConfigurationManager.restorationFinished(this);

        setAssociatedXMLIDForHelp(NEWSFEED_HELP_ID);

        startNewsFeed();
    }

    public static void displayTab() {
        WindowsConfigurationManager.restoreWindow(NEWSFEED_UUID);
    }

    public void startNewsFeed() {
        newsFeedController.start();
    }

    public void updateNewsFeed() {
        newsFeedController.updateNewsFeed();
    }

    public void closeTab() {
        stopNewsFeed();
        instance = null;
    }

    public void addInfoBar(TextBox infoBar) {
        setInfoBar(infoBar);
    }

    public void addToolBar(ToolBar toolBar) {
        setToolBar(toolBar);
    }

    public void addMenuBar(MenuBar menuBar) {
        setMenuBar(menuBar);
    }

    public SwingScilabWindow getParentWindow() {
        return SwingScilabWindow.allScilabWindows.get(getParentWindowId());
    }

    public SimpleTab getAsSimpleTab() {
        return this;
    }

    private MenuBar createMenuBar() {
        MenuBar menuBar = ScilabMenuBar.createMenuBar();

        Menu fileMenu = ScilabMenu.createMenu();
        fileMenu.setText(UiDataMessages.FILE);
        fileMenu.setMnemonic('F');
        fileMenu.add(CloseAction.createMenuItem());
        fileMenu.add(UpdateAction.createMenuItem());
        menuBar.add(fileMenu);

        Menu helpMenu = ScilabMenu.createMenu();
        helpMenu.setText(NewsFeedUIMessages.NEWS_FEED_HELP_LABEL);
        helpMenu.setMnemonic('?');
        helpMenu.add(HelpAction.createMenuItem());
        menuBar.add(helpMenu);

        return menuBar;
    }

    private ToolBar createToolBar() {
        ToolBar toolBar = ScilabToolBar.createToolBar();
        SwingScilabToolBar swingScilabToolBar = (SwingScilabToolBar) toolBar.getAsSimpleToolBar();
        swingScilabToolBar.add(UpdateAction.createPushButton());
        swingScilabToolBar.addSeparator();
        swingScilabToolBar.add(HelpAction.createPushButton());
        return toolBar;
    }

    private void registerClosingOperation() {
        ClosingOperationsManager.registerClosingOperation(this, new ClosingOperationsManager.ClosingOperation() {

            @Override
            public int canClose() {
                return 1;
            }

            @Override
            public void destroy() {
                closeTab();
            }

            @Override
            public String askForClosing(final List<SwingScilabDockablePanel> list) {
                return null;
            }

            @Override
            public void updateDependencies(List<SwingScilabDockablePanel> list,
                                           ListIterator<SwingScilabDockablePanel> it) {
            }
        });

        ClosingOperationsManager.addDependencyWithRoot(this);
    }

    private void stopNewsFeed() {
        newsFeedController.stop();
    }
}
