/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import java.util.logging.Logger;

import javax.swing.JButton;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabComponent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;

/**
 * Launch the modelica compiler configuration TCL UI
 */
@SuppressWarnings(value = { "serial" })
public final class InitModelicaAction extends SimulationNotRunningAction {
    /** the name */
    public static final String NAME = XcosMessages.INIT_MODELICA;
    /** no icon */
    public static final String SMALL_ICON = "";
    /** no mnemonic */
    public static final int MNEMONIC_KEY = 0;
    /** no accelerator */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * Constructor
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     */
    public InitModelicaAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create a button for a graph toolbar
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     * @return the button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, InitModelicaAction.class);
    }

    /**
     * Create a menu for a graph menubar
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, InitModelicaAction.class);
    }

    /**
     * Action associated
     *
     * @param e
     *            the event
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

        graph.info(XcosMessages.INITIALIZING_MODELICA_COMPILER);

        final StringBuilder command = new StringBuilder();
        command.append("scs_m = scicos_new(\"0x").append(Long.toHexString(graph.getRootDiagram().getUID())).append("\"); ");
        command.append("xcosConfigureModelica(scs_m); ");

        try {
            asynchronousScilabExec((ActionEvent e1) -> { graph.info(XcosMessages.EMPTY_INFO); }, command.toString());
        } catch (InterpreterException e1) {
            Logger.getLogger(InitModelicaAction.class.getName()).severe(e.toString());
        }
    }
}
