/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.utils;

import java.awt.Image;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import org.flexdock.docking.props.DockablePropertySet;
import org.flexdock.docking.props.PropertyChangeListenerFactory;
import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;

/**
 * Handle window bar update on tab activation.
 */
public final class BarUpdater implements PropertyChangeListener {
    private static final PropertyChangeListener LISTENER_INSTANCE = new BarUpdater();

    /**
     * Factory for the bar update on tab activation handler.
     */
    public static class UpdateBarFactory extends PropertyChangeListenerFactory {

        @Override
        public PropertyChangeListener getListener() {
            return LISTENER_INSTANCE;
        }

    }

    /**
     * Constructor
     */
    private BarUpdater() { }

    /**
     * Local update for MenuBar and ToolBar
     * Called when a Dock is complete.
     * @param parentWindowsID : the ID of the window we want to update.
     * @param newMenuBar the new MenuBar to display.
     * @param newToolBar the new ToolBar to display.
     * @param newInfoBar the new InfoBar to display.
     * @param newWindowTitle the new Title to display
     */
    public static void updateBars(String parentWindowsID, MenuBar newMenuBar, ToolBar newToolBar, TextBox newInfoBar, String newWindowTitle) {
        updateBars(parentWindowsID, newMenuBar, newToolBar, newInfoBar, newWindowTitle, null);
    }

    /**
     * Local update for MenuBar and ToolBar
     * Called when a Dock is complete.
     * @param parentWindowsID : the ID of the window we want to update.
     * @param newMenuBar the new MenuBar to display.
     * @param newToolBar the new ToolBar to display.
     * @param newInfoBar the new InfoBar to display.
     * @param newWindowTitle the new Title to display
     * @param newIcon the new windows icon
     */
    public static void updateBars(String parentWindowsID, MenuBar newMenuBar, ToolBar newToolBar, TextBox newInfoBar, String newWindowTitle, Image newIcon) {
        SwingScilabWindow parentWindow = SwingScilabWindow.allScilabWindows.get(parentWindowsID);
        if (parentWindow != null) {
            parentWindow.addMenuBar(newMenuBar);
            parentWindow.addToolBar(newToolBar);
            parentWindow.addInfoBar(newInfoBar);
            parentWindow.setTitle(newWindowTitle);
            /** The following line is used to update the menubar, toolbar, ... displayed on screen */
            parentWindow.getRootPane().revalidate();
            if (OS.get() == OS.MAC) { /* Fix bug #11787 */
                parentWindow.repaint();
            }
            if (newIcon != null) {
                parentWindow.setIconImage(newIcon);
            }
        }
    }

    /**
     * Update the bar on activation event.
     *
     * @param evt the event emitted by a {@link SwingScilabTab}
     * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
     */
    @Override
    public void propertyChange(PropertyChangeEvent evt) {
        if (evt.getSource() instanceof SwingScilabTab
                && evt.getPropertyName().equals(DockablePropertySet.ACTIVE)
                && evt.getNewValue().equals(Boolean.TRUE)) {
            SwingScilabTab tab = (SwingScilabTab) evt.getSource();

            BarUpdater.updateBars(tab.getParentWindowId(),
                                  tab.getMenuBar(),
                                  tab.getToolBar(),
                                  tab.getInfoBar(),
                                  tab.getName(),
                                  tab.getWindowIcon());
        }
    }
}
