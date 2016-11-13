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
import java.awt.event.ActionListener;
import java.util.logging.Logger;

import javax.swing.JButton;
import javax.swing.Timer;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.graph.actions.base.OneBlockDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;

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

    private int counter;
    private Timer displayTimer;

    /**
     * @param scilabGraph
     *            graph
     */
    public StartAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
        setEnabled(GraphActionManager.getEnable(StartAction.class));

        char[] msg = (XcosMessages.SIMULATION_IN_PROGRESS + XcosMessages.DOTS).toCharArray();
        final int minimalMsgLen = XcosMessages.SIMULATION_IN_PROGRESS.length();

        counter = 0;
        displayTimer = new Timer(1000, e -> {
            counter = (counter + 1) % (XcosMessages.DOTS.length() + 1);
            ((XcosDiagram) scilabGraph).info(new String(msg, 0, minimalMsgLen + counter));
        });
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

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        updateUI(true);
        displayTimer.start();

        final String cmd = createSimulationCommand(graph);
        final ActionListener action = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                displayTimer.stop();
                updateUI(false);
            }
        };

        try {
            asynchronousScilabExec(action, cmd);
        } catch (InterpreterException e1) {
            final Logger log = Logger.getLogger(StartAction.class.getName());
            log.warning(e1.getMessage());
        }
    }

    /**
     * Create the command String
     *
     * @param diagram
     *            the working diagram
     * @return the command string
     */
    private String createSimulationCommand(final XcosDiagram diagram) {
        String cmd;
        final StringBuilder command = new StringBuilder();

        /*
         * Log compilation info
         */
        final Logger log = Logger.getLogger(StartAction.class.getName());
        log.finest("start simulation");

        JavaController controller = new JavaController();
        int[] debugLevel = new int[1];
        controller.getObjectProperty(diagram.getUID(), diagram.getKind(), ObjectProperties.DEBUG_LEVEL, debugLevel);
        command.append(buildCall("scicos_debug", debugLevel[0])).append("; ");

        /*
         * Export the schema on `scs_m` and simulate
         */
        command.append("scs_m = scicos_new(\"0x").append(Long.toHexString(diagram.getUID())).append("\"); ");
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
