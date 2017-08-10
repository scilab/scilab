/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
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

import java.awt.Cursor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.Action;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.utils.BlockPositioning;

/**
 * External action
 */
@SuppressWarnings(value = { "serial" })
public final class ExternalAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = "";
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    final String localCommand;

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            the graph
     */
    public ExternalAction(ScilabGraph scilabGraph, String command) {
        super(scilabGraph);

        this.localCommand = command;
    }

    /**
     * Copy constructor
     *
     * @param action
     *            the action to copy
     * @param graph
     *            the graph
     */
    public ExternalAction(ExternalAction action, XcosDiagram graph) {
        super(graph);

        putValue(Action.NAME, action.getValue(Action.NAME));
        localCommand = action.localCommand;
    }

    static private void reset_view(final XcosDiagram graph, final BasicBlock block, boolean start) {
        if (block != null) {
            block.setLocked(start);
        }

        int cursor;
        if (start) {
            cursor = Cursor.WAIT_CURSOR;
        } else {
            cursor = Cursor.DEFAULT_CURSOR;
        }

        graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(cursor));
        graph.setCellsLocked(start);

        graph.getView().clear(graph.getCurrentRoot(), true, true);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);
        final StringBuilder command = new StringBuilder();

        final BasicBlock block;
        final ActionListener callback;

        /*
         * Export the whole graph, to update all the sub-graphs on demand.
         */
        command.append("scs_m = scicos_new(\"0x").append(Long.toHexString(graph.getUID())).append("\"); \n");

        /*
         * Then export the selected block
         */
        Object cell = graph.getSelectionCell();
        if (cell instanceof BasicBlock) {
            block = (BasicBlock) cell;
            command.append("blk = scicos_new(\"0x").append(Long.toHexString(block.getUID())).append("\"); \n");
            command.append(localCommand).append('\n');
            command.append("if exists('blk') then xcosCellCreated(blk); end\n");
        } else {
            block = null;
        }



        /*
         * Import the updated block
         */
        if (block != null) {
            callback = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    // Now update the Block
                    graph.getModel().beginUpdate();
                    try {
                        ScicosObjectOwner last = XcosCellFactory.getLastCreated();
                        if (last != null && last.getUID() != 0l) {
                            JavaController controller = new JavaController();

                            BasicBlock modified = XcosCellFactory.createBlock(controller, last);
                            if (modified != null) {
                                block.updateBlockSettings(controller, graph, modified);
                            }
                        }
                        BlockPositioning.updateBlockView(graph, block);
                    } finally {
                        graph.getModel().endUpdate();
                        reset_view(graph, block, false);
                    }
                }
            };
        } else {
            callback = (ActionEvent ev) -> {
                reset_view(graph, block, false);
            };
        }

        try {
            reset_view(graph, block, true);
            ScilabInterpreterManagement.asynchronousScilabExec(callback, command.toString());
        } catch (InterpreterException e1) {
            reset_view(graph, block, false);
        }
    }
}
