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

import java.awt.event.ActionEvent;
import java.util.logging.Logger;

import javax.swing.Action;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.xcos.graph.XcosDiagram;

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

    private static final Logger LOG = Logger.getLogger(ExternalAction.class.getName());

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

    @Override
    public void actionPerformed(ActionEvent e) {

        // FIXME: implement the external action
        //        final XcosDiagram graph = (XcosDiagram) getGraph(e);
        //        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        //        if (handler == null) {
        //            return;
        //        }
        //
        //        final BasicBlock block;
        //        final ActionListener callback;
        //        try {
        //            /*
        //             * Export the whole diagram, to update all the sub-diagrams on demand.
        //             */
        //            handler.writeDiagram(graph.getRootDiagram());
        //
        //            /*
        //             * Then export the selected block
        //             */
        //            Object cell = graph.getSelectionCell();
        //            if (cell instanceof BasicBlock) {
        //                block = (BasicBlock) cell;
        //                handler.writeBlock(block);
        //            } else {
        //                block = null;
        //            }
        //
        //            /*
        //             * Import the updated block
        //             */
        //            if (block != null) {
        //                callback = new ActionListener() {
        //                    @Override
        //                    public void actionPerformed(ActionEvent e) {
        //                        try {
        //
        //                            final BasicBlock modifiedBlock = handler.readBlock();
        //                            block.updateBlockSettings(modifiedBlock);
        //
        //                            graph.fireEvent(new mxEventObject(XcosEvent.ADD_PORTS, XcosConstants.EVENT_BLOCK_UPDATED, block));
        //                        } catch (ScicosFormatException e1) {
        //                            LOG.severe(e1.getMessage());
        //                        } finally {
        //                            handler.release();
        //                        }
        //                    }
        //                };
        //            } else {
        //                callback = new ActionListener() {
        //                    @Override
        //                    public void actionPerformed(ActionEvent e) {
        //                        handler.release();
        //                    }
        //                };
        //            }
        //
        //            ScilabInterpreterManagement.asynchronousScilabExec(callback, localCommand);
        //        } catch (InterpreterException e2) {
        //            LOG.warning(e2.toString());
        //
        //            handler.release();
        //        }
    }
}
