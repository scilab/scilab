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

package org.scilab.modules.gui.plotbrowser.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.plotbrowser.PlotBrowser;
import org.scilab.modules.gui.plotbrowser.MessagesPlotBrowser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.utils.ClosingOperationsManager;

/**
 * Close action
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class CloseAction extends CommonCallBack {

    private static final KeyStroke KEY = ScilabKeyStroke.getKeyStroke("OSSCKEY W");

    /**
     * Default constructor
     */
    public CloseAction() {
        super(null);
    }

    /**
     * @return a menuitem associated with this action
     */
    public static MenuItem createMenu() {
        MenuItem item = ScilabMenuItem.createMenuItem();
        SwingScilabMenuItem swingItem = (SwingScilabMenuItem) item.getAsSimpleMenuItem();
        swingItem.setAction(new CloseAction());
        swingItem.setText(MessagesPlotBrowser.close);
        swingItem.setAccelerator(KEY);

        return item;
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        PlotBrowser.getPlotBrowser().close();
    }
}
