/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.filebrowser;

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.ui_data.tabfactory.FileBrowserTab;
import org.scilab.modules.ui_data.tabfactory.FileBrowserTabFactory;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * The file browser
 * @author Calixte DENIZET
 */
public final class ScilabFileBrowser {

    private static SwingScilabFileBrowser instance;

    static {
        ScilabTabFactory.getInstance().addTabFactory(FileBrowserTabFactory.getInstance());
    }

    /**
     * Constructor
     */
    private ScilabFileBrowser() {
        TextBox infobar = ScilabTextBox.createTextBox();
        instance = new SwingScilabFileBrowser();
        instance.addInfoBar(infobar);
        instance.setTitle(UiDataMessages.FILE_BROWSER);
    }

    /**
     * Create a file browser (as tab) instance
     * @return the instance
     */
    public static SwingScilabFileBrowser createFileBrowserTab() {
        if (instance == null) {
            new ScilabFileBrowser();
        }

        return instance;
    }

    /**
     * @return true if an instance of BrowseVar already exists.
     */
    public static boolean isFileBrowserOpened() {
        return instance != null;
    }

    /**
     * Set the base directory
     * @param dir the base directory
     */
    public static void setBaseDir(String dir) {
        if (instance != null) {
            instance.setBaseDir(dir);
        }
    }

    /**
     * Get the variable browser singleton with specified columns title.
     * @return the File Browser
     */
    public static SwingScilabFileBrowser getFileBrowser() {
        if (instance == null) {
            boolean success = WindowsConfigurationManager.restoreUUID(SwingScilabFileBrowser.FILEBROWSERUUID);
            if (!success) {
                FileBrowserTab.getFileBrowserInstance();
                SwingScilabWindow window = SwingScilabWindow.createWindow(true);
                window.addTab(instance);
                window.setLocation(0, 0);
                window.setSize(500, 500);
                window.setVisible(true);
            }
        } else {
            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, instance);
            window.setVisible(true);
            window.toFront();
        }

        return instance;
    }

    /**
     * Close File Browser
     */
    public static void closeFileBrowser() {
        if (instance != null) {
            instance.close();
            instance = null;
        }
    }

    /**
     * Close File Browser
     */
    public void close() {
        instance = null;
    }

    /**
     * {@inheritDoc}
     */
    public void setVisible(boolean status) {
        instance.setVisible(status);
    }
}
