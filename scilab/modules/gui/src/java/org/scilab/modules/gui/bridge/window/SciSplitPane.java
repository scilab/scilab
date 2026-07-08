/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.bridge.window;

import org.flexdock.docking.DockingManager;
import org.flexdock.docking.DockingPort;
import org.flexdock.docking.defaults.DockingSplitPane;

public class SciSplitPane extends DockingSplitPane {

    public static void initStatic() {
        DockingManager.setDockingStrategy(SciSplitPane.class, new SciDockingStrategy());
    }

    public SciSplitPane(DockingPort port, String region) {
        super(port, region);
        setContinuousLayout(true);
    }
};

