/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.bridge.window;

import java.awt.BorderLayout;
import java.awt.Component;

import org.flexdock.docking.DockingPort;
import org.scilab.modules.gui.bridge.tab.SwingScilabPanel;

public class SwingScilabStaticWindow extends SwingScilabWindow {

    public SwingScilabStaticWindow() {
        super();
        setLayout(new BorderLayout());        
    }
    
    /**
     * @return number of objects (tabs) docked in this window
     */
    @Override
    public int getNbDockedObjects() {
        return 1;
    }
    
    /**
     * Close the window
     * @see org.scilab.modules.gui.window.SimpleWindow#close()
     */
    @Override
    public void close() {
        try {
            setVisible(false);
            dispose();
        } catch (IllegalStateException e) {
            enableInputMethods(false);
        }
        allScilabWindows.remove(windowUID);
    }

    public DockingPort getDockingPort() {
        return null;
    }

    public void addTab(SwingScilabPanel tab) {
        add((Component) tab, BorderLayout.CENTER);
    }

    public void removeTabs(SwingScilabPanel[] tabs) {
        removeAll();
    }
}
