/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * Copyright (C) 2010 - Calixte DENIZET
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.event.ActionEvent;
import java.util.Iterator;

import javax.swing.AbstractAction;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingConstants;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.DockingPort;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.view.View;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;

/**
 * Action executed when the user clicks on the "Undock button"
 * @author Vincent COUVERT
 * @author Calixte DENIZET
 */
public class SciUndockingAction extends AbstractAction {

    private static final long serialVersionUID = 3906773111254753683L;

    private static final int UNDOCK_OFFSET = 30;

    private SwingScilabTab associatedTab;

    /**
     * Constructor
     * @param tab the associated tab
     */
    public SciUndockingAction(SwingScilabTab tab) {
        associatedTab = tab;
    }

    /**
     * What do I have to do when the event accurs
     * @param arg0 the event
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent arg0) {
        /** Create a new Window to dock the tab into */
        SwingScilabWindow newWindow = new SwingScilabWindow();

        /** Save the tab dimensions to set them back after docking */
        Size oldtabSize = associatedTab.getDims();
        /** Save the old parent Window position to use it to set the new Window position */
        Position oldWindowPosition = associatedTab.getParentWindow().getPosition();
        /* If we undock a tab contained in view with two elements, then
           the two elements will be alone, so we remove the actions. */
        DockingPort port = DockingManager.getMainDockingPort(associatedTab);
        Iterator iter = port.getDockables().iterator();
        if (port.getDockables().size() == 2) {
            while (iter.hasNext()) {
                Object d = iter.next();
                if (d instanceof View) {
                    View view = (View) d;
                    view.setActionBlocked(DockingConstants.CLOSE_ACTION, true);
                    view.setActionBlocked("undock", true);
                }
            }
        }

        /** Undock/Dock the tab */
        DockingManager.undock((Dockable) associatedTab);
        iter = port.getDockables().iterator();
        if (iter.hasNext()) {
            SwingScilabTab tab = (SwingScilabTab) iter.next();
            BarUpdater.updateBars(tab.getParentWindowId(), tab.getMenuBar(), tab.getToolBar(), tab.getInfoBar(), tab.getName(), tab.getWindowIcon());
        }
        DockingManager.dock(associatedTab, newWindow.getDockingPort());
        BarUpdater.updateBars(associatedTab.getParentWindowId(), associatedTab.getMenuBar(), associatedTab.getToolBar(), associatedTab.getInfoBar(), associatedTab.getName(), associatedTab.getWindowIcon());
        ActiveDockableTracker.requestDockableActivation(associatedTab);

        /** New Window properties */
        newWindow.setPosition(new Position(oldWindowPosition.getX() + UNDOCK_OFFSET, oldWindowPosition.getY() + UNDOCK_OFFSET));

        newWindow.setVisible(associatedTab.isVisible());

        /** Set new Window dimensions so that the tab has the same dimensions as before */
        Size windowSize = newWindow.getDims();
        Size newTabSize = associatedTab.getDims();
        newWindow.setDims(new Size((windowSize.getWidth() - newTabSize.getWidth()) + oldtabSize.getWidth(),
                                   (windowSize.getHeight() - newTabSize.getHeight()) + oldtabSize.getHeight()));

        associatedTab.setParentWindowId(newWindow.getId());
        associatedTab.requestFocus();
    }
}
