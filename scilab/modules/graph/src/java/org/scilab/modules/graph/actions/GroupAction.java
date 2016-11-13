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

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.MultiSelectionDependantAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;

import com.mxgraph.swing.util.mxGraphActions;

/**
 * Group any blocks and ease the manipulation of them.
 */
@SuppressWarnings(value = { "serial" })
public class GroupAction extends MultiSelectionDependantAction {
    /** Name of the action */
    public static final String NAME = ScilabGraphMessages.GROUP;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_G;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            The associated graph
     */
    public GroupAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create the menu associated with this action.
     * @param scilabGraph the associated graph
     * @return The associated menu
     */
    public static MenuItem groupMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, GroupAction.class);
    }

    /**
     * Action to be done
     * @param e Event descriptor
     */
    public void actionPerformed(ActionEvent e) {
        mxGraphActions.getGroupAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
                e.getID(), e.getActionCommand()));
    }

}
