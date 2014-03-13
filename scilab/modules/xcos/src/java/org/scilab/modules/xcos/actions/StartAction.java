/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.logging.Logger;

import javax.swing.JButton;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.graph.actions.base.OneBlockDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Start the simulation
 */
@SuppressWarnings(value = { "serial" })
public final class StartAction extends OneBlockDependantAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.START;
    /** Icon name of the action */
    public static final String SMALL_ICON = "media-playback-start";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * @param scilabGraph
     *            graph
     */
    public StartAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
        setEnabled(GraphActionManager.getEnable(StartAction.class));
    }

    /**
     * @param scilabGraph
     *            graph
     * @return push button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, StartAction.class);
    }

    /**
     * @param scilabGraph
     *            graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, StartAction.class);
    }

    /**
     * Action !!!
     *
     * @param e
     *            the source event
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);
        String cmd;

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        if (handler == null) {
            return;
        }

        updateUI(true);

        try {
            cmd = createSimulationCommand(graph, handler);
        } catch (IOException ex) {
            Logger.getLogger(StartAction.class.getName()).severe(ex.toString());
            updateUI(false);

            handler.release();
            return;
        }

        final ActionListener action = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateUI(false);
                graph.getEngine().setCompilationNeeded(false);

                handler.release();
            }
        };

        try {
            asynchronousScilabExec(action, cmd);
        } catch (InterpreterException e1) {
            e1.printStackTrace();
            updateUI(false);

            handler.release();
        }
    }

    /**
     * Create the command String
     *
     * @param diagram
     *            the working diagram
     * @param handler
     *            the handler use to communicate with Scilab
     * @return the command string
     * @throws IOException
     *             when temporary files must not be created.
     */
    private String createSimulationCommand(final XcosDiagram diagram, final ScilabDirectHandler handler) throws IOException {
        String cmd;
        final StringBuilder command = new StringBuilder();

        /*
         * Log compilation info
         */
        final Logger log = Logger.getLogger(StartAction.class.getName());
        log.finest("start simulation");

        /*
         * Import a valid scs_m structure into Scilab
         */
        handler.writeDiagram(diagram);
        command.append(buildCall("scicos_debug", diagram.getScicosParameters().getDebugLevel()));

        /*
         * Simulate
         */
        command.append("xcos_simulate(scs_m, 4); ");

        cmd = command.toString();
        return cmd;
    }

    /**
     * Update the UI depending on the action selected or not
     *
     * @param started
     *            the started status
     */
    public void updateUI(boolean started) {
        GraphActionManager.setEnable(StartAction.class, !started);
        GraphActionManager.setEnable(StopAction.class, false);
        ((XcosDiagram) getGraph(null)).setReadOnly(started);

        if (started) {
            ((XcosDiagram) getGraph(null)).info(XcosMessages.SIMULATION_IN_PROGRESS);
        } else {
            ((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
        }
    }
}
