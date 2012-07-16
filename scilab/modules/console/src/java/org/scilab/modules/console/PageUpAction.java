/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.console;

import java.awt.event.ActionEvent;

import javax.swing.BoundedRangeModel;
import javax.swing.JScrollPane;

import com.artenum.rosetta.core.action.AbstractConsoleAction;

/**
 * Scroll up the console
 * This event is configured in configuration.xml file
 * @author Calixte DENIZET
 */
public class PageUpAction extends AbstractConsoleAction {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public PageUpAction() {
        super();
    }

    /**
     * Threats the event
     * @param e the action event that occurred
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
        JScrollPane jSP = ((SciOutputView) configuration.getOutputView()).getConsole().getJScrollPane();
        BoundedRangeModel brm = jSP.getVerticalScrollBar().getModel();
        brm.setValue(brm.getValue() - jSP.getVerticalScrollBar().getBlockIncrement(-1));
    }
}
