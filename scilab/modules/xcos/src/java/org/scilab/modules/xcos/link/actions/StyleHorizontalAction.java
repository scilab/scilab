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

import com.mxgraph.util.mxConstants;

/**
 * Implement the set link horizontal action
 */
@SuppressWarnings(value = { "serial" })
public class StyleHorizontalAction extends StyleAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.LINK_STYLE_HORIZONTAL;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_H;

    /**
     * Default constructor the associated graph
     *
     * @param scilabGraph
     *            the graph to associate
     */
    public StyleHorizontalAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * @param scilabGraph
     *            graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, StyleHorizontalAction.class);
    }

    /**
     * Action !!!
     *
     * @param e
     *            params
     * @see org.scilab.modules.xcos.link.actions.StyleAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        final Object[] links = getLinks();

        graph.getModel().beginUpdate();
        try {
            graph.setCellStyles(mxConstants.STYLE_NOEDGESTYLE, "0", links);
            graph.setCellStyles(mxConstants.STYLE_EDGE,
                                mxConstants.EDGESTYLE_ELBOW, links);
            graph.setCellStyles(mxConstants.STYLE_ELBOW,
                                mxConstants.ELBOW_HORIZONTAL, links);

            reset(graph, links);
        } finally {
            graph.getModel().endUpdate();
        }
    }
}
