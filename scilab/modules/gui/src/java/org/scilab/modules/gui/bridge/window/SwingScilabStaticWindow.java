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

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CLOSEREQUESTFCN__;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import org.flexdock.docking.DockingPort;
import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.bridge.tab.SwingScilabPanel;
import org.scilab.modules.gui.bridge.tab.SwingScilabStaticPanel;
import org.scilab.modules.gui.events.callback.CommonCallBack;

public class SwingScilabStaticWindow extends SwingScilabWindow {

    private SwingScilabStaticPanel panel;

    public SwingScilabStaticWindow() {
        super();
        setLayout(new BorderLayout());
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                String closeRequestFcn = (String) GraphicController.getController().getProperty(panel.getId(), __GO_CLOSEREQUESTFCN__);
                if (!closeRequestFcn.equals("")) {
                    String closeCommand = "if exists(\"gcbo\") then %oldgcbo = gcbo; end;" + "gcbo = getcallbackobject(" + panel.getId() + ");" + closeRequestFcn
                                          + ";if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;";
                    InterpreterManagement.putCommandInScilabQueue(closeCommand);
                } else {
                    //GraphicController.getController().deleteObject(panel.getId());
                    CommonCallBack callback = panel.getCallback();
                    if (callback != null) {
                        callback.actionPerformed(null);
                    } else {
                        GraphicController.getController().removeRelationShipAndDelete(panel.getId());
                    }
                }
            }
        });
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
        this.panel = (SwingScilabStaticPanel) tab;
    }

    public void removeTabs(SwingScilabPanel[] tabs) {
        removeAll();
    }
}
