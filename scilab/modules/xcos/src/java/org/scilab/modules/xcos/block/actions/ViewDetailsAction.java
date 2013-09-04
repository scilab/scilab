/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions;

import java.awt.event.ActionEvent;
import java.util.logging.Logger;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * View the details of the action
 */
@SuppressWarnings(value = { "serial" })
public final class ViewDetailsAction extends VertexSelectionDependantAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.DETAILS;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * Constructor
     *
     * @param scilabGraph
     *            graph
     */
    public ViewDetailsAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * @param scilabGraph
     *            graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, ViewDetailsAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        Object[] selectedCells = graph.getSelectionCells();

        // if no cells are selected : Do nothing
        if (selectedCells.length == 0) {
            return;
        }

        for (int i = 0; i < selectedCells.length; ++i) {
            if ((selectedCells[i] instanceof BasicBlock) && !(selectedCells[i] instanceof SplitBlock)) {
                BasicBlock instance = (BasicBlock) selectedCells[i];
                viewDetails(instance);
            }
        }
    }

    /**
     * View the data details
     *
     * @param data
     *            the selected block
     */
    private void viewDetails(BasicBlock data) {
        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        if (handler == null) {
            return;
        }

        try {
            /*
             * Export data
             */
            handler.writeBlock(data);

            /*
             * Build and execute the command
             */
            final String cmd = "tree_show(" + ScilabDirectHandler.BLK + ");";
            ScilabInterpreterManagement.synchronousScilabExec(cmd);

        } catch (InterpreterException e1) {
            Logger.getLogger(ViewDetailsAction.class.getName()).severe(e1.toString());
        } finally {
            handler.release();
        }
    }
}
