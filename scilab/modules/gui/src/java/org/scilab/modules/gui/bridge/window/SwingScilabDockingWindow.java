/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
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

package org.scilab.modules.gui.bridge.window;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Iterator;
import java.util.Set;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.DockingPort;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.docking.defaults.DefaultDockingPort;
import org.flexdock.docking.defaults.DefaultDockingStrategy;
import org.flexdock.docking.drag.effects.EffectsManager;
import org.flexdock.docking.drag.preview.GhostPreview;
import org.flexdock.docking.event.hierarchy.DockingPortTracker;
import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.tab.SwingScilabPanel;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.SciDockingListener;

public class SwingScilabDockingWindow extends SwingScilabWindow {

    static {
        DefaultDockingStrategy.setDefaultResizeWeight(0.5);
        DefaultDockingStrategy.keepConstantPercentage(true);
    }

    private DefaultDockingPort sciDockingPort;
    private SciDockingListener sciDockingListener;

    public SwingScilabDockingWindow() {
        super();
        /* Create automatically a docking port associated to the window */
        sciDockingPort = new DefaultDockingPort();

        EffectsManager.setPreview(new GhostPreview());

        /* The docking port is the center of the Layout of the Window */
        super.add(sciDockingPort, java.awt.BorderLayout.CENTER);
        sciDockingListener = new SciDockingListener();
        sciDockingPort.addDockingListener(sciDockingListener);
        sciDockingListener.setAssociatedWindowId(windowUID);

        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                ClosingOperationsManager.startClosingOperation(SwingScilabDockingWindow.this);
            }
        });
    }

    /**
     * Gets the docking port associated to the window (created by default at window creation)
     * @return the docking port associated to the window
     */
    public DockingPort getDockingPort() {
        //return (DockingPort) centerFrame.getContentPane();
        return sciDockingPort;
    }

    /**
     * Add a Scilab tab to a Scilab window
     * @param newTab the Scilab tab to add to the Scilab window
     * @see org.scilab.modules.gui.window.Window#addTab(org.scilab.modules.gui.tab.Tab)
     */
    public void addTab(SwingScilabPanel newTab) {
        SwingScilabDockablePanel tab = (SwingScilabDockablePanel) newTab;
        tab.setParentWindowId(this.windowUID);
        DockingManager.dock(tab, this.getDockingPort());
        ActiveDockableTracker.requestDockableActivation(tab);
    }

    /**
     * Remove a Scilab tab from a Scilab window
     * @param tabs the Scilab tabs to remove from the Scilab window
     * @see org.scilab.modules.gui.window.Window#removeTab(org.scilab.modules.gui.tab.Tab)
     */
    public void removeTabs(SwingScilabPanel[] tabs) {
        for (SwingScilabDockablePanel tab : (SwingScilabDockablePanel[]) tabs) {
            DockingManager.unregisterDockable((Dockable) tab);
            tab.close();
            DockingManager.close(tab);
        }

        if (getDockingPort() == null || getDockingPort().getDockables().isEmpty()) {
            // remove xxxBars
            if (toolBar != null) {
                ((SwingScilabToolBar) toolBar).removeAll();
            }
            if (menuBar != null) {
                ((SwingScilabMenuBar) menuBar).removeAll();
            }

            // clean all
            this.removeAll();
            close();
        } else {
            /* Make sur a Tab is active */
            Set<SwingScilabDockablePanel> docks = sciDockingPort.getDockables();
            Iterator<SwingScilabDockablePanel> it = docks.iterator();
            ActiveDockableTracker.requestDockableActivation(it.next());
        }
    }

    /**
     * @return number of objects (tabs) docked in this window
     */
    @Override
    public int getNbDockedObjects() {
        return sciDockingPort.getDockables().size();
    }

    /**
     * Close the window
     * @see org.scilab.modules.gui.window.SimpleWindow#close()
     */
    @Override
    public void close() {
        try {
            dispose();
            // disable docking port
            ActiveDockableTracker.getTracker(this).setActive(null);
            if (sciDockingPort != null) {
                sciDockingPort.removeDockingListener(sciDockingListener);
                sciDockingPort = null;
                sciDockingListener = null;
            }
            DockingPortTracker.remove(this);
        } catch (IllegalStateException e) {
            enableInputMethods(false);
        }
        allScilabWindows.remove(windowUID);
    }
}
