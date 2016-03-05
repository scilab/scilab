/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
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
package org.scilab.modules.gui.plotbrowser;

import javax.swing.SwingUtilities;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.ScilabWindow;

/**
* Plot Browser.
* shows all objects plotted.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class PlotBrowser {
    private static PlotBrowser instance;
    public static SwingPlotBrowser plotBrowserTab;
    public static Integer currentObjectID;

    /**
    * Constructor.
    *
    * @param objectID Enters the identification of object.
    */
    public PlotBrowser(Integer objectID) {
        TextBox infobar = ScilabTextBox.createTextBox();
        plotBrowserTab = new SwingPlotBrowser(objectID);
        plotBrowserTab.addInfoBar(infobar);
    }

    /**
     * Create the plot browser (as tab) instance.
     *
     * @param objectID Enters the identification of object.
     * @return the instance.
     */
    public static SwingPlotBrowser createPlotBrowserTab(Integer objectID) {
        if (instance == null) {
            instance = new PlotBrowser(objectID);
        }

        return plotBrowserTab;
    }

    /**
     * Checks if the plot browser is open.
     *
     * @return true if an instance of PlotBrowser already exists.
     */
    public static boolean isPlotBrowserOpened() {
        return instance != null;
    }

    /**
     * Get the plot browser singleton
     * @return the plot browser
     */
    public static PlotBrowser getPlotBrowser() {
        return instance;
    }

    /**
    * Initializes the PlotBrowser with the docking system.
    *
    * @param objectID Enters the identification of object.
    * @return Shows the PlotBrowser was generated.
    */
    public static PlotBrowser getPlotBrowser(Integer objectID) {
        currentObjectID = objectID;
        if (instance == null) {
            boolean success = WindowsConfigurationManager.restoreUUID(SwingPlotBrowser.PLOTBROWSERUUID);
            if (!success) {
                PlotBrowserTab.getPlotBrowserInstance(objectID);
                SwingScilabWindow window = SwingScilabWindow.createWindow(true);
                window.addTab(plotBrowserTab);
                window.setLocation(0, 0);
                window.setSize(300, 700);
                window.setVisible(true);
            }
        } else {
            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass
                                       (SwingScilabWindow.class, plotBrowserTab);
            window.setVisible(true);
            window.toFront();
            new SwapFigure(objectID);
        }
        return instance;
    }

    /**
     * Close PlotBrowser
     */
    public static void closePlotBrowser() {
        if (instance != null) {
            instance = null;
        }
    }

    /**
     * Close PlotBrowser
     */
    public void close() {
        ClosingOperationsManager.startClosingOperationWithoutSave((SwingScilabDockablePanel) plotBrowserTab);
    }

    /**
     * {@inheritDoc}
     */
    public void setVisible(boolean status) {
        plotBrowserTab.setVisible(status);
    }
}