/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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
package org.scilab.modules.ui_data.variablebrowser;

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
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
     * Opens Variable Browser
     * @return the Variable Browser
     */
    public static VariableBrowser openVariableBrowser() {
        if (instance == null) {
            boolean success = WindowsConfigurationManager.restoreUUID(SwingScilabVariableBrowser.VARBROWSERUUID);
            if (!success) {
                VariableBrowserTab.getVariableBrowserInstance();
                SwingScilabWindow window = SwingScilabWindow.createWindow(true);
                window.addTab(browserTab);
                window.setLocation(0, 0);
                window.setSize(500, 500);
                window.setVisible(true);
                window.toFront();
            }
        }
        return instance;
    }

    /**
     * Set Variable Browser data
     * @param data : data from scilab (type, name, size, ...)
     */
    public static void setVariableBrowserData(final Object[][] data) {
        if (instance != null) {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    if (instance != null) {
                        instance.setData(data);
                    }
                }
            });
        }
    }

    /**
     * Update Variable Browser window
     */
    public static void updateVariableBrowser() {
        if (instance != null) {
            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, browserTab);
            window.repaint();
        }
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
     * Get the variable browser singleton
     * @return the Variable Browser
     */
    public static VariableBrowser getVariableBrowser() {
        return instance;
    }

    /**
     * Close Variable Browser
     */
    public void close() {
        ClosingOperationsManager.startClosingOperationWithoutSave((SwingScilabDockablePanel) browserTab);
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
