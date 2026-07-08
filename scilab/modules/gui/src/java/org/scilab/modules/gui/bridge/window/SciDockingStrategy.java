/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.bridge.window;

import javax.swing.JSplitPane;

import org.flexdock.docking.DockingPort;
import org.flexdock.docking.defaults.DefaultDockingStrategy;

public class SciDockingStrategy extends DefaultDockingStrategy {
    protected JSplitPane createSplitPaneImpl(DockingPort base, String region) {
        return new SciSplitPane(base, region);
    }
    /*protected JSplitPane createSplitPaneImpl(DockingPort base, String region, float percent) {
        return new SciSplitPane(base, region, percent);
    }*/
    protected DockingPort createDockingPortImpl(DockingPort base) {
        return new SciDockingPort();
    }
};
