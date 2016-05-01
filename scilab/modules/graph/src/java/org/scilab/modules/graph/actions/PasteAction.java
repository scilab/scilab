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

import javax.swing.TransferHandler;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;

import com.mxgraph.swing.handler.mxGraphTransferHandler;

/**
 * Paste manager
 *
 * This action is enabled by the {@link CutAction} and {@link CopyAction}.
 */
@SuppressWarnings(value = { "serial" })
public final class PasteAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = ScilabGraphMessages.PASTE;
    /** Icon name of the action */
    public static final String SMALL_ICON = "edit-paste";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_V;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Constructor
     * @param scilabGraph corresponding Scilab Graph
     */
    public PasteAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        setEnabled(TransferHandler.getPasteAction().isEnabled());
    }

    /**
     * Create a menu for a graph menubar
     * @param scilabGraph corresponding Scilab Graph
     * @return the menu
     */
    public static MenuItem pasteMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, PasteAction.class);
    }

    /**
     * Action associated
     * @param e the event
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        mxGraphTransferHandler.getPasteAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
                e.getID(), e.getActionCommand()));
    }
}
