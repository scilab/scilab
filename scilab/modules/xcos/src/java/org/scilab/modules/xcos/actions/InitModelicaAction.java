/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import java.util.logging.Logger;

import javax.swing.JButton;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.utils.XcosMessages;

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

        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        if (handler == null) {
            return;
        }

        graph.info(XcosMessages.INITIALIZING_MODELICA_COMPILER);

        handler.writeDiagram(graph.getRootDiagram());

        final String cmd = buildCall("xcosConfigureModelica");

        final ActionListener action = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                graph.info(XcosMessages.EMPTY_INFO);
                handler.release();
            }
        };

        try {
            asynchronousScilabExec(action, cmd);
        } catch (InterpreterException e1) {
            Logger.getLogger(InitModelicaAction.class.getName()).severe(e.toString());
            handler.release();
        }
    }
}
