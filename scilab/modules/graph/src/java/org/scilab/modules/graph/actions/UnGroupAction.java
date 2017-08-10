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
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;

import com.mxgraph.swing.util.mxGraphActions;

/**
 * Ungroup any blocks and ease the manipulation of them.
 */
@SuppressWarnings(value = { "serial" })
public class UnGroupAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = ScilabGraphMessages.UNGROUP;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_G;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = KeyEvent.SHIFT_DOWN_MASK
            | Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            The associated graph
     */
    public UnGroupAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create the menu associated with this action.
     * @param scilabGraph the associated graph
     * @return The associated menu
     */
    public static MenuItem ungroupMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, UnGroupAction.class);
    }

    /**
     * Action to be done
     * @param e Event descriptor
     */
    public void actionPerformed(ActionEvent e) {
        mxGraphActions.getUngroupAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
                e.getID(), e.getActionCommand()));
    }

}
