/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.graph.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.lang.ref.WeakReference;

import javax.swing.JButton;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.ActionConstraint;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;

import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUndoManager;

/**
 * Redo manager
 */
@SuppressWarnings(value = { "serial" })
public class RedoAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = ScilabGraphMessages.REDO;
    /** Icon name of the action */
    public static final String SMALL_ICON = "edit-redo";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_Y;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Manage enable modification
     */
    private final class RedoConstraint extends ActionConstraint {
        private final WeakReference<ScilabGraph> scilabGraph;

        /**
         * Default constructor
         * @param scilabGraph the graph to work on
         */
        public RedoConstraint(ScilabGraph scilabGraph) {
            this.scilabGraph = new WeakReference<ScilabGraph>(scilabGraph);
        }

        /**
         * @param action
         *            the current action
         * @param scilabGraph
         *            the associated graph
         * @see org.scilab.modules.graph.actions.base.ActionConstraint#install(org.scilab.modules.graph.actions.base.DefaultAction,
         *      org.scilab.modules.graph.ScilabGraph)
         */
        @Override
        public void install(DefaultAction action, ScilabGraph scilabGraph) {
            super.install(action, scilabGraph);

            registerAsListener(scilabGraph.getUndoManager());
        }

        /**
         * @param manager the associated UndoManager
         */
        public void registerAsListener(mxUndoManager manager) {
            manager.addListener(mxEvent.UNDO, this);
            manager.addListener(mxEvent.REDO, this);
            manager.addListener(mxEvent.ADD, this);
            manager.addListener(mxEvent.CLEAR, this);
        }

        /**
         * To be checked
         * @param sender the event sender
         * @param evt the current event
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(Object sender, mxEventObject evt) {
            final ScilabGraph graph = scilabGraph.get();
            if (graph == null) {
                return;
            }

            boolean canRedo = graph.getUndoManager().canRedo();
            super.setEnabled(canRedo);
        }
    }

    /**
     * Constructor
     * @param scilabGraph corresponding Scilab Graph
     */
    public RedoAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        RedoConstraint c = new RedoConstraint(scilabGraph);
        c.install(this, scilabGraph);
    }

    /**
     * Create a button for a graph toolbar
     * @param scilabGraph corresponding Scilab Graph
     * @return the button
     */
    public static JButton redoButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, RedoAction.class);
    }

    /**
     * Create a menu for a graph menubar
     * @param scilabGraph corresponding Scilab Graph
     * @return the menu
     */
    public static MenuItem redoMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, RedoAction.class);
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
        final ScilabGraph graph = getGraph(e);

        graph.removeUndoHandler();
        graph.getUndoManager().redo();
        graph.registerUndoHandler();

        // revalidate the graph
        graph.getAsComponent().clearCellOverlays();
        graph.getAsComponent().validateGraph();
    }
}
