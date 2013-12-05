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

package org.scilab.modules.gui.plotbrowser;

import org.scilab.modules.gui.ged.StartGED;

/**
 * Manages the action of the buttons on the toolbar and menubar.
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class StartPlotBrowser {

    /**
    * Initializes the Plot Browser.
    */
    public static void plotBrowser(Integer uid) {
        if (StartGED.enableNewGed) {
            PlotBrowser.getPlotBrowser(uid);
        }
    }
}
