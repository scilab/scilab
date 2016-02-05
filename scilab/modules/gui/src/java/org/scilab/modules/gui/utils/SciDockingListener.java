/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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

package org.scilab.modules.gui.utils;

import java.util.Iterator;
import java.util.Set;

import javax.swing.JLayeredPane;
import javax.swing.JPanel;
import javax.swing.JRootPane;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.defaults.DefaultDockingPort;
import org.flexdock.docking.event.DockingEvent;
import org.flexdock.docking.event.DockingListener;
import org.flexdock.docking.floating.frames.FloatingDockingPort;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;

/**
 * Listener for docking operations in Scilab
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class SciDockingListener implements DockingListener {

    private String associatedScilabWindowId;

    /**
     * Default constructor
     */
    public SciDockingListener() {
        super();
    }

    /**
     * Handling docking canceled events
     * @param e the event
     * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
     */
    public void dockingCanceled(DockingEvent e) {
        debug("dockingCanceled");
    }

    /**
     * Handling docking complete events
     * @param e the event
     * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
     */
    public void dockingComplete(DockingEvent e) {
        debug("dockingComplete");
        String newId = null;

        DockingListener[] newListeners = e.getNewDockingPort().getDockingListeners();
        SwingScilabDockablePanel dockedTab = (SwingScilabDockablePanel) e.getDockable();
        if (newListeners.length == 2) {
            /* This docking port has been created when the parent window were created */
            /* So this docking port has a sciDockingListener */
            newId = ((SciDockingListener) newListeners[1]).getAssociatedWindowId();
        } else { /* Docking port created by Flexdock itself */
            /* Retrieve the ID of the parent window */
            Set<Dockable> allDockables = e.getNewDockingPort().getDockables();
            /* Have to find an other dockable than the one we just docked */
            Iterator<Dockable> it =  allDockables.iterator();
            Dockable dock = it.next();

            if (it.hasNext()) { /** More than one dockable --> already existing docking port */
                while (e.getDockable() == dock) {
                    dock = it.next();
                }
                newId = ((SwingScilabDockablePanel) dock).getParentWindowId();
            } else { /** Create a new Window to display the tab inside it */

                DefaultDockingPort dockingPort = ((DefaultDockingPort) e.getOldDockingPort());
                JPanel parentPanel = (JPanel) dockingPort.getParent();
                JLayeredPane parentLayeredPane = (JLayeredPane) parentPanel.getParent();
                JRootPane parentRootPane = (JRootPane) parentLayeredPane.getParent();

                /* Computes new position for the created Window */
                int offsetX = dockingPort.getX() + parentPanel.getX() + parentLayeredPane.getX() + parentRootPane.getX();
                int offsetY = dockingPort.getY() + parentPanel.getY() + parentLayeredPane.getY() + parentRootPane.getY();
                int newX = -offsetX;
                int newY = -offsetY;
                if (e.getNewDockingPort() instanceof FloatingDockingPort) {
                    newX += ((FloatingDockingPort) e.getNewDockingPort()).getParent().getParent().getParent().getX();
                    newY += ((FloatingDockingPort) e.getNewDockingPort()).getParent().getParent().getParent().getY();
                } else {
                    newX += ((DefaultDockingPort) e.getNewDockingPort()).getParent().getParent().getParent().getX();
                    newY += ((DefaultDockingPort) e.getNewDockingPort()).getParent().getParent().getParent().getY();
                }

                SwingScilabWindow newWindow = SwingScilabWindow.createWindow(true);
                newWindow.setPosition(new Position(newX, newY));
                newWindow.setDims(SwingScilabWindow.allScilabWindows.get(associatedScilabWindowId).getDims());
                DockingManager.dock(dockedTab, newWindow.getDockingPort());
                newWindow.setVisible(true);
                newId = newWindow.getId();
            }
        }
        dockedTab.setParentWindowId(newId);
        dockedTab.requestFocus();
    }

    /**
     * Handling docking started events
     * @param e the event
     * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
     */
    public void dragStarted(DockingEvent e) {
        // DEBUG
        debug("dragStarted");
    }

    /**
     * Handling drop started events
     * @param e the event
     * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
     */
    public void dropStarted(DockingEvent e) {
        // DEBUG
        debug("dropStarted");
    }

    /**
     * Handling undocking complete events
     * @param e the event
     * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
     */
    public void undockingComplete(DockingEvent e) {
        // DEBUG
        debug("undockingComplete");

        // If the dock is empty, we close the parent Window
        if (e.getOldDockingPort().getDockables().isEmpty()) {
            SwingScilabWindow.allScilabWindows.get(associatedScilabWindowId).close();
        }
    }

    /**
     * Handling undocking started events
     * @param e the event
     * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
     */
    public void undockingStarted(DockingEvent e) {
        // DEBUG
        debug("undockingStarted");
    }

    /**
     * Set the window object associated to this docking listener
     * @param id the id of the window associated
     */
    public void setAssociatedWindowId(String id) {
        this.associatedScilabWindowId = id;
    }

    /**
     * Get the window object associated to this docking listener
     * @return the id of the window associated
     */
    public String getAssociatedWindowId() {
        return this.associatedScilabWindowId;
    }

    /**
     * Display debug information
     * @param method the name of the method called
     */
    private void debug(String method) {
        //System.out.println("["+method+"] on Window "+associatedScilabWindowId);
    }

}
