/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;

import org.scilab.modules.gui.bridge.CallScilabBridge;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;

/**
 * Action executed when the user clicks on the "Help button"
 * @author Calixte DENIZET
 */
public class SciHelpOnComponentAction extends AbstractAction {

    private static final long serialVersionUID = 1L;

    private SwingScilabTab associatedTab;

    /**
     * Constructor
     * @param tab the associated tab
     */
    public SciHelpOnComponentAction(SwingScilabTab tab) {
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
