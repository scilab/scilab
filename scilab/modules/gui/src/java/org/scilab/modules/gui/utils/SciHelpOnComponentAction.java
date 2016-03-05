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

import javax.swing.AbstractAction;

import org.scilab.modules.gui.bridge.CallScilabBridge;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;

/**
 * Action executed when the user clicks on the "Help button"
 * @author Calixte DENIZET
 */
public class SciHelpOnComponentAction extends AbstractAction {

    private static final long serialVersionUID = 1L;

    private SwingScilabDockablePanel associatedTab;

    /**
     * Constructor
     * @param tab the associated tab
     */
    public SciHelpOnComponentAction(SwingScilabDockablePanel tab) {
        associatedTab = tab;
    }

    /**
     * What do I have to do when the event accurs
     * @param arg0 the event
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent arg0) {
        String id = associatedTab.getAssociatedXMLIDForHelp();
        CallScilabBridge.openHelp(id);
    }
}
