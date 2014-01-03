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

/**
* Manager Figure exchange.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class SwapFigure {

    public SwapFigure(Integer objectID) {
        SwingPlotBrowser.pReceive.removeAll();
        new ListCreator(objectID);
        SwingPlotBrowser.pReceive.revalidate();
        SwingPlotBrowser.pReceive.repaint();
    }
}