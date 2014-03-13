/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.JButton;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * New Diagram creation
 */
@SuppressWarnings(value = { "serial" })
public final class NewDiagramAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.NEW_DIAGRAM;
    /** Icon name of the action */
    public static final String SMALL_ICON = "document-new";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_N;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated Scilab Graph
     */
    public NewDiagramAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create a menu item for the graph menubar
     *
     * @param scilabGraph
     *            associated Scilab Graph
     * @return the menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, NewDiagramAction.class);
    }

    /**
     * Create a button for the graph toolbar
     *
     * @param scilabGraph
     *            associated Scilab Graph
     * @return the button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, NewDiagramAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        Xcos.getInstance().open(null, null);
    }

}
