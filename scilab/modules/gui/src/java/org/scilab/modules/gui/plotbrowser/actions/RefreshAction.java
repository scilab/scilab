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

import javax.swing.ImageIcon;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.plotbrowser.PlotBrowser;
import org.scilab.modules.gui.plotbrowser.StartPlotBrowser;
import org.scilab.modules.gui.plotbrowser.SwingPlotBrowser;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;

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
    public static PushButton createButton(String title) {
        PushButton button = ScilabPushButton.createPushButton();
        ((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(new RefreshAction(title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(FindIconHelper.findIcon("view-refresh"));
        ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(imageIcon);

        return button;
    }
}