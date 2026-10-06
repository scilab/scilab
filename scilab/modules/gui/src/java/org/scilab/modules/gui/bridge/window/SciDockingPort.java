/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.bridge.window;

import java.awt.Component;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.defaults.DefaultDockingPort;
import org.scilab.modules.commons.gui.ScilabCefPanel;

public class SciDockingPort extends DefaultDockingPort {

    private static final long serialVersionUID = 1L;

    public static void initStatic() {
        DockingManager.setDockingStrategy(SciDockingPort.class, new SciDockingStrategy());
    }

    // disable ScilabCefPanel#removeNotify() on dock()
    @Override
    public boolean dock(Dockable dockable, String region) {
        ScilabCefPanel.setDockInProgress(true);
        boolean d = super.dock(dockable, region);
        ScilabCefPanel.setDockInProgress(false);
        return d;
    }

    // disable ScilabCefPanel#removeNotify() on undock()
    @Override
    public boolean undock(Component comp) {
        ScilabCefPanel.setDockInProgress(true);
        boolean dedock = super.undock(comp);
        ScilabCefPanel.setDockInProgress(false);
        return dedock;
    }

    // No masking during a drag on purpose: hiding the browser makes JCEF call
    // setWindowVisibility(false), which suspends Chromium's rendering. It would
    // then only re-layout once restored - a long, visible refresh when docking
    // into a bigger area. Left visible, it is resized and reflows as we drag.
};

