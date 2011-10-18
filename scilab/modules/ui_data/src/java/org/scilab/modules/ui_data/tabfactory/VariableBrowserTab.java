/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.tabfactory;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.ui_data.variablebrowser.ScilabVariableBrowser;

/**
 * Class to create SciNotes instances
 * @author Calixte DENIZET
 */
public class VariableBrowserTab {

    /**
     * @param uuid the uuid to restore
     * @return a new SciNotes instance
     */
    public static SwingScilabTab getVariableBrowserInstance() {
	final SwingScilabTab varbrowser = ScilabVariableBrowser.createVarBrowserTab();
	ScilabTabFactory.getInstance().addToCache(varbrowser);
        
	ClosingOperationsManager.registerClosingOperation(varbrowser, new ClosingOperationsManager.ClosingOperation() {

                public boolean canClose() {
                    return true;
                }

                public void destroy() {
                    ScilabVariableBrowser.closeVariableBrowser();
                }

                public String askForClosing() {
                    return null;
                }
            });

        ClosingOperationsManager.addDependencyWithRoot(varbrowser);

        return varbrowser;
    }
}
