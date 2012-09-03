/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data.variablebrowser;

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.ui_data.BrowseVar;
import org.scilab.modules.ui_data.tabfactory.VariableBrowserTab;
import org.scilab.modules.ui_data.tabfactory.VariableBrowserTabFactory;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 *
 * Class ScilabVariableBrowser
 * Implements a ScilabWindow containing Variable Browser (JTable)
 *
 */
public final class ScilabVariableBrowser implements VariableBrowser {

    private static ScilabVariableBrowser instance;
    private static SwingScilabVariableBrowser browserTab;

    static {
        ScilabTabFactory.getInstance().addTabFactory(VariableBrowserTabFactory.getInstance());
    }

    /**
     * Constructor
     * @param columnNames the columns title.
     */
    private ScilabVariableBrowser() {
        TextBox infobar = ScilabTextBox.createTextBox();
        browserTab = new SwingScilabVariableBrowser(BrowseVar.COLUMNNAMES, BrowseVar.COLUMNSALIGNMENT);
        browserTab.addInfoBar(infobar);
        browserTab.setTitle(UiDataMessages.VARIABLE_BROWSER);
    }

    public static SwingScilabVariableBrowser createVarBrowserTab() {
        if (instance == null) {
            instance = new ScilabVariableBrowser();
        }

        return browserTab;
    }

    /**
     * @return true if an instance of BrowseVar already exists.
     */
    public static boolean isBrowseVarOpened() {
        return instance != null;
    }

    /**
     * Retrieve Singleton
     * @param columnNames : columns title
     * @param data : data from scilab (type, name, size, ...)
     * @return the Variable Browser
     */
    public static VariableBrowser getVariableBrowser(boolean update, final Object[][] data) {
        final VariableBrowser variableBrowser = getVariableBrowser(update);
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                variableBrowser.setData(data);
            }
        });
        return variableBrowser;
    }

    /**
     * Get the variable browser singleton
     * @return the Variable Browser
     */
    public static VariableBrowser getVariableBrowser() {
        return instance;
    }

    /**
     * Get the variable browser singleton with specified columns title.
     * @param columnNames : the columns title
     * @return the Variable Browser
     */
    public static VariableBrowser getVariableBrowser(boolean update) {
        if (instance == null) {
            boolean success = WindowsConfigurationManager.restoreUUID(SwingScilabVariableBrowser.VARBROWSERUUID);
            if (!success) {
                VariableBrowserTab.getVariableBrowserInstance();
                SwingScilabWindow window = (SwingScilabWindow) ScilabWindow.createWindow().getAsSimpleWindow();
                window.addTab(browserTab);
                window.setLocation(0, 0);
                window.setSize(500, 500);
                window.setVisible(true);
            }
        } else {
            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, browserTab);
            if (!update) {
                window.setVisible(true);
                window.toFront();
            }
        }

        return instance;
    }

    /**
     * Close Variable Browser
     */
    public static void closeVariableBrowser() {
        if (instance != null) {
            instance = null;
        }
    }

    /**
     * Close Variable Browser
     */
    public void close() {
        ClosingOperationsManager.startClosingOperationWithoutSave((SwingScilabTab) browserTab);
    }

    /**
     * Set data displayed in JTable
     * @param data : data to be displayed in JTable
     */
    public void setData(Object[][] data) {
        browserTab.setData(data);
    }


    public void updateRowFiltering() {
        browserTab.updateRowFiltering();
    }

    /**
     * {@inheritDoc}
     */
    public void setVisible(boolean status) {
        //super.setVisible(status);
        browserTab.setVisible(status);
    }
}
