/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged;

import javax.swing.SwingUtilities;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.ScilabWindow;

/**
* Main properties window - Light GED.
* Receive the classes of graphic objects (JPanels).
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Inspector {
    private static Inspector instance;
    private static SwingInspector inspectorTab;

    /**
    * Constructor.
    *
    * @param select Indicates which property window will open.
    * @param objectID Enters the identification of object.
    */
    public Inspector(String select , String objectID) {
        TextBox infobar = ScilabTextBox.createTextBox();
	inspectorTab = new SwingInspector(select , objectID);
        inspectorTab.addInfoBar(infobar);
        //inspectorTab.setTitle("Inspector");
    }

    /**
     * Create the inspector (as tab) instance
     * @param select Indicates which property window will open initially.
     * @param objectID Enters the identification of object.
     * @return the instance.
     */
    public static SwingInspector createInspectorTab(String select, String objectID) {
        if (instance == null) {
            instance = new Inspector(select , objectID);
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
    * Initializes the Inspector with the docking system.
    *
    * @param select Indicates which property window will open initially.
    * @param objectID Enters the identification of object.
    * @return Shows the lightGED was generated.
    */
    public static Inspector getInspector(String select, String objectID) {
        if (instance == null) {
            boolean success = WindowsConfigurationManager.restoreUUID(SwingInspector.INSPECTORUUID);
            if (!success) {
                InspectorTab.getInspectorInstance(select, objectID);
                SwingScilabWindow window = (SwingScilabWindow) ScilabWindow.createWindow().getAsSimpleWindow();
                window.addTab(inspectorTab);
                window.setLocation(0, 0);
                window.setSize(370, 500);
                window.setVisible(true);
            }
        } else {
            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, (SwingInspector) inspectorTab);
            window.setVisible(true);
            window.toFront();
        }

        return instance;
    }

    /**
     * Close Inspector
     */
    public static void closeInspector() {
        if (instance != null) {
            instance.close();
            instance = null;
        }
    }

    /**
     * Close Inspector
     */
    public void close() {
        instance = null;
    }

    /**
     * {@inheritDoc}
     */
    public void setVisible(boolean status) {
        inspectorTab.setVisible(status);
    }
}