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
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Management of "Diagram Saving"
 */
@SuppressWarnings(value = { "serial" })
public final class SaveAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.SAVE;
    /** Icon name of the action */
    public static final String SMALL_ICON = "media-floppy";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_S;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated Xcos diagram
     */
    public SaveAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create menu for saving
     *
     * @param scilabGraph
     *            associated Xcos diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, SaveAction.class);
    }

    /**
     * Create toolbar button for saving
     *
     * @param scilabGraph
     *            associated Xcos diagram
     * @return the button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, SaveAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        ((XcosDiagram) getGraph(null)).getRootDiagram().saveDiagram();
    }
}
