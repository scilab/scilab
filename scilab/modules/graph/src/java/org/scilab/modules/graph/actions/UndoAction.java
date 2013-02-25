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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.lang.ref.WeakReference;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.ActionConstraint;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;

import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUndoManager;

/**
 * Undo manager
 */
@SuppressWarnings(value = { "serial" })
public class UndoAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = ScilabGraphMessages.UNDO;
    /** Icon name of the action */
    public static final String SMALL_ICON = "edit-undo";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_Z;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Manage enable modification
     */
    private final class UndoConstraint extends ActionConstraint {
        private final WeakReference<ScilabGraph> scilabGraph;

        /**
         * Default constructor
         * @param scilabGraph the associated scilab graph
         */
        public UndoConstraint(ScilabGraph scilabGraph) {
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
        private void registerAsListener(mxUndoManager manager) {
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

            boolean canUndo = graph.getUndoManager().canUndo();
            super.setEnabled(canUndo);
        }
    }

    /**
     * Constructor
     * @param scilabGraph corresponding Scilab Graph
     */
    public UndoAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        UndoConstraint c = new UndoConstraint(scilabGraph);
        c.install(this, scilabGraph);
    }

    /**
     * Create a button for a graph toolbar
     * @param scilabGraph corresponding Scilab Graph
     * @return the button
     */
    public static PushButton undoButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, UndoAction.class);
    }

    /**
     * Create a menu for a graph menubar
     * @param scilabGraph corresponding Scilab Graph
     * @return the menu
     */
    public static MenuItem undoMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, UndoAction.class);
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
        graph.getUndoManager().undo();
        graph.registerUndoHandler();
    }

}
