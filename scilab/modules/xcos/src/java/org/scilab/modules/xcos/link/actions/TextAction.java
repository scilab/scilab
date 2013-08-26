/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.link.actions;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Set the text of a link.
 *
 * @deprecated since 5.4.0
 */
@Deprecated
@SuppressWarnings(value = { "serial" })
public class TextAction extends StyleAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.EDIT;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_F2;

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            the current graph
     */
    public TextAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create the associated menu
     *
     * @param scilabGraph
     *            graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, TextAction.class);
    }

    /**
     * Action
     *
     * @param e
     *            parameters
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        final Object[] links = graph.getAllEdges(graph.getSelectionCells());

        if (links.length > 0) {
            comp.startEditingAtCell(links[0], e);
        }
    }

}
