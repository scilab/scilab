/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

import java.awt.event.ActionEvent;
import java.util.Iterator;

import javax.swing.AbstractAction;
import javax.swing.Action;

import org.flexdock.docking.DockingConstants;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.DockingPort;
import org.flexdock.view.View;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;

/**
 * Action executed when the user clicks on the "Close button"
 * It's a just wrapper to handle the removal of the button
 * when only one View is docked.
 * @author Calixte DENIZET
 */
public class SciClosingAction extends AbstractAction {

    private static final long serialVersionUID = 1L;

    private SwingScilabDockablePanel associatedTab;
    private Action action;

    /**
     * Constructor
     * @param tab the associated tab
     */
    public SciClosingAction(SwingScilabDockablePanel tab) {
        this(tab, null);
    }

    /**
     * Constructor
     * @param tab the associated tab
     * @param action the action
     */
    public SciClosingAction(SwingScilabDockablePanel tab, Action action) {
        this.associatedTab = tab;
        this.action = action;
    }

    /**
     * @return the closing action
     */
    public Action getAction() {
        return action;
    }

    public void clean() {
        this.associatedTab = null;
        this.action = null;
    }

    /**
     * What do I have to do when the event accurs
     * @param arg0 the event
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent arg0) {
        if (action == null) {
            ClosingOperationsManager.startClosingOperation(associatedTab);
        } else {
            oldActionPerformed(arg0);
        }
    }

    /**
     * TODO: This method should be removed and ClosingOperation should be registered instead.
     * @param arg0 the event
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void oldActionPerformed(ActionEvent arg0) {
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

        action.actionPerformed(arg0);
    }
}
