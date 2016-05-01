/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.link.actions;

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.xcos.graph.swing.handler.SelectionCellsHandler;
import org.scilab.modules.xcos.link.BasicLink;

/**
 * base class for changing the link style
 */
public abstract class StyleAction extends DefaultAction {
    /** Accelerator key for all children actions */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            the graph to associate
     */
    public StyleAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * @return the current selected links on the graph
     */
    protected BasicLink[] getLinks() {
        Object[] cells = getGraph(null).getSelectionModel().getCells();
        List<BasicLink> links = new ArrayList<BasicLink>();

        for (Object object : cells) {
            if (object instanceof BasicLink) {
                links.add((BasicLink) object);
            }
        }

        return links.toArray(new BasicLink[links.size()]);
    }

    protected void reset(final ScilabGraph graph, final Object[] edges) {
        final SelectionCellsHandler selectionCellsHandler = (SelectionCellsHandler) graph.getAsComponent().getSelectionCellsHandler();

        for (Object edge : edges) {
            graph.resetEdge(edge);
            selectionCellsHandler.clearCellHandler(edge);
        }
    }
}
