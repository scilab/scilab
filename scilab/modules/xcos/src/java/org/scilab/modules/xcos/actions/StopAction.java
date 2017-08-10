/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;

import javax.swing.JButton;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Stop the simulation
 */
@SuppressWarnings(value = { "serial" })
public final class StopAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.STOP;
    /** Icon name of the action */
    public static final String SMALL_ICON = "process-stop";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * @param scilabGraph
     *            graph
     */
    public StopAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
        setEnabled(false);
    }

    /**
     * @param scilabGraph
     *            graph
     * @return push button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, StopAction.class);
    }

    /**
     * @param scilabGraph
     *            graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, StopAction.class);
    }

    /**
     * Action !!!
     *
     * @param e
     *            source event
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        if (!GraphActionManager.getEnable(StartAction.class)) {
            JavaController.end_simulation();

            graph.info(XcosMessages.EMPTY_INFO);
            GraphActionManager.setEnable(StartAction.class, true);
            GraphActionManager.setEnable(StopAction.class, false);
            graph.setReadOnly(false);
        }
    }
}
