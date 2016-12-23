/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Chenfeng ZHU
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
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.utils.XcosRoute;

import com.mxgraph.util.mxConstants;

/**
 * Implement the set link optimal action.
 */
@SuppressWarnings(value = { "serial" })
public class StyleOptimalAction extends StyleAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.LINK_STYLE_OPTIMAL;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_O;

    /**
     * Default constructor the associated graph
     *
     * @param scilabGraph
     *            the graph to associate
     */
    public StyleOptimalAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * @param scilabGraph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, StyleOptimalAction.class);
    }

    /**
     * Action.
     *
     * @param e
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
            double scale = graph.getView().getScale();
            graph.getView().setScale(1.0);
            graph.setCellStyles(mxConstants.STYLE_NOEDGESTYLE, "1", links);
            reset(graph, links);
            this.updateLinkOptimal(graph, links);
            graph.getView().setScale(scale);
        } finally {
            graph.getModel().endUpdate();
        }
    }

    /**
     * Update the style of links one by one.
     *
     * @param graph
     * @param links
     */
    public void updateLinkOptimal(XcosDiagram graph, Object[] links) {
        Object[] allChildCells = graph.getChildCells(graph.getDefaultParent());
        XcosRoute route = new XcosRoute();
        for (Object o : links) {
            if (o instanceof BasicLink) {
                BasicLink link = (BasicLink) o;
                route.updateRoute(link, allChildCells, graph);
            }
        }
    }

}
