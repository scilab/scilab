/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.logging.Logger;

import javax.swing.Action;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosEvent;

import com.mxgraph.util.mxEventObject;

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
        final XcosDiagram graph = (XcosDiagram) getGraph(e);
        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        if (handler == null) {
            return;
        }

        final BasicBlock block;
        final ActionListener callback;
        try {
            /*
             * Export the whole diagram, to update all the sub-diagrams on demand.
             */
            handler.writeDiagram(graph.getRootDiagram());

            /*
             * Then export the selected block
             */
            Object cell = graph.getSelectionCell();
            if (cell instanceof BasicBlock) {
                block = (BasicBlock) cell;
                handler.writeBlock(block);
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
                        try {

                            final BasicBlock modifiedBlock = handler.readBlock();
                            block.updateBlockSettings(modifiedBlock);

                            graph.fireEvent(new mxEventObject(XcosEvent.ADD_PORTS, XcosConstants.EVENT_BLOCK_UPDATED, block));
                        } catch (ScicosFormatException e1) {
                            LOG.severe(e1.getMessage());
                        } finally {
                            handler.release();
                        }
                    }
                };
            } else {
                callback = new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        handler.release();
                    }
                };
            }

            ScilabInterpreterManagement.asynchronousScilabExec(callback, localCommand);
        } catch (InterpreterException e2) {
            LOG.warning(e2.toString());

            handler.release();
        }
    }
}
