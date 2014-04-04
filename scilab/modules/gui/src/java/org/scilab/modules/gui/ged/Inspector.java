/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.ged;

import javax.swing.SwingUtilities;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.ScilabWindow;

/**
* Main properties window - Light GED.
* Receive the classes of graphic objects (JPanels).
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Inspector {
    private static Inspector instance;
    private static SwingInspector inspectorTab;
    private static GEDView gedView;

    /**
    * Constructor.
    *
    * @param selected Indicates which property window will open.
    * @param objectID Enters the identification of object.
    */
    public Inspector(Integer objectID) {
        TextBox infobar = ScilabTextBox.createTextBox();
        inspectorTab = new SwingInspector(objectID);
        inspectorTab.addInfoBar(infobar);
        gedView = new GEDView();
    }

    /**
     * Create the inspector (as tab) instance
     * @param selected Indicates which property window will open initially.
     * @param objectID Enters the identification of object.
     * @return the instance.
     */
    public static SwingInspector createInspectorTab(Integer objectID) {
        if (instance == null) {
            instance = new Inspector(objectID);
        }

        return inspectorTab;
    }

    /**
     * Checks if the lightGED is open.
     *
     * @return true if an instance of Inspector already exists.
     */
    public static boolean isInspectorOpened() {
        return instance != null;
    }

    /**
     * Get the inspector singleton
     * @return the inspector
     */
    public static Inspector getInspector() {
        return instance;
    }

    /**
    * Initializes the Inspector with the docking system.
    *
    * @param objectID Enters the identification of object.
    * @return Shows the lightGED was generated.
    */
    public static Inspector getInspector(Integer objectID) {
        if (instance == null) {
            boolean success = WindowsConfigurationManager.restoreUUID(SwingInspector.INSPECTORUUID);
            if (!success) {
                InspectorTab.getInspectorInstance(objectID);
                SwingScilabWindow window = SwingScilabWindow.createWindow(true);
                window.addTab(inspectorTab);
                window.setLocation(0, 0);
                window.setSize(300, 700);
                window.setVisible(true);
            }
        } else {
            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass
                                       (SwingScilabWindow.class, inspectorTab);
            window.setVisible(true);
            window.toFront();
            new SwapObject(objectID);
        }
        return instance;
    }

    /**
     * Close Inspector
     */
    public static void closeInspector() {
        if (instance != null) {
            instance = null;
            gedView.close();
        }
    }

    /**
     * Close Inspector
     */
    public void close() {
        ClosingOperationsManager.startClosingOperationWithoutSave((SwingScilabDockablePanel) inspectorTab);
        gedView.close();
    }

    /**
     * {@inheritDoc}
     */
    public void setVisible(boolean status) {
        inspectorTab.setVisible(status);
    }
}
