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

package org.scilab.modules.gui.plotbrowser.actions;

import javax.swing.ImageIcon;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.plotbrowser.PlotBrowser;
import org.scilab.modules.gui.plotbrowser.StartPlotBrowser;
import org.scilab.modules.gui.plotbrowser.SwingPlotBrowser;

/**
 * Refresh action class
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public final class RefreshAction extends CommonCallBack {

    /**
     * Constructor
     * @param name the name of the action
     */
    public RefreshAction(String name) {
        super(name);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        StartPlotBrowser.plotBrowser(PlotBrowser.currentObjectID);
        SwingPlotBrowser.pReceive.revalidate();
        SwingPlotBrowser.pReceive.repaint();
    }

    /**
     * Create a button for a tool bar
     * @param title tooltip for the button
     * @return the button
     */
    public static SwingScilabPushButton createButton(String title) {
        SwingScilabPushButton button = new SwingScilabPushButton();
        button.addActionListener(new RefreshAction(title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(FindIconHelper.findIcon("view-refresh"));
        button.setIcon(imageIcon);

        return button;
    }
}