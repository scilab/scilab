/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.xcos.port;

import java.util.List;

import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxMultiplicity;

/**
 * Validate the connection between ports for a rule.
 *
 * The {@link #check(mxGraph, Object, Object, Object, int, int)} method will
 * return an error message if the link cannot be created. Otherwise it will
 * return an empty string.
 */
public class PortCheck extends mxMultiplicity {

    private final Class <? extends mxCell > sourceTemplate;
    private final List < Class <? extends mxCell >> targetTemplateList;
    private final String errorMessage;

    /**
     * Construct a new connection rule.
     *
     * @param sourceTemplate
     *            The source ports
     * @param targetTemplate
     *            The target ports
     * @param errorMessage
     *            The associated error message.
     */
    public PortCheck(Class <? extends mxCell > sourceTemplate,
                     List < Class <? extends mxCell >> targetTemplate, String errorMessage) {
        // We completely override mxMultiplicity
        super(true, null, null, null, 0, null, null, null, null, false);
        this.sourceTemplate = sourceTemplate;
        this.targetTemplateList = targetTemplate;
        this.errorMessage = errorMessage;
        this.max = "n";

    }

    /**
     * Check for the rule.
     *
     * @param graph
     *            the graph
     * @param edge
     *            the edge
     * @param source
     *            the start
     * @param target
     *            the end
     * @param sourceOut
     *            number of connections at source.
     * @param targetIn
     *            number of connections at target
     * @return An error message or null if the link is valid
     *
     * @see com.mxgraph.view.mxMultiplicity#check(com.mxgraph.view.mxGraph,
     *      java.lang.Object, java.lang.Object, java.lang.Object, int, int)
     */
    @Override
    public String check(mxGraph graph, Object edge, Object source,
                        Object target, int sourceOut, int targetIn) {

        // check that source and target has no connections (removing the current
        // edge)
        final mxIGraphModel model = graph.getModel();
        final int sourceCount = getEdgeCount(model, source, edge);
        final int targetCount = getEdgeCount(model, target, edge);

        if (sourceCount > 0 || targetCount > 0) {
            if (errorMessage
                    .compareTo(XcosMessages.LINK_ERROR_ALREADY_CONNECTED) == 0) {
                return XcosMessages.LINK_ERROR_ALREADY_CONNECTED;
            }
            return null;
        }

        if (isTypeCompatible(source, target)) {
            return null;
        }

        return errorMessage;
    }

    /**
     * Returns the number of incoming and outgoing edges, ignoring the given
     * edge.
     *
     * @param model
     *            Graph model that contains the connection data.
     * @param cell
     *            Cell whose edges should be counted.
     * @param ignoredEdge
     *            Object that represents an edge to be ignored.
     * @return Returns the number of incoming and outgoing edges.
     * @see mxGraphModel#getDirectedEdgeCount(mxIGraphModel, Object, boolean,
     *      Object)
     */
    private int getEdgeCount(mxIGraphModel model, Object cell,
                             Object ignoredEdge) {
        int count = 0;
        int edgeCount = model.getEdgeCount(cell);

        for (int i = 0; i < edgeCount; i++) {
            Object edge = model.getEdgeAt(cell, i);

            if (edge != ignoredEdge) {
                count++;
            }
        }

        return count;
    }

    /**
     * Check for port compatibility.
     *
     * @param firstPort
     *            the first port to check
     * @param secondPort
     *            the second port to check
     * @return true when compatible, false otherwise
     */
    private boolean isTypeCompatible(Object firstPort, Object secondPort) {
        /*
         * if the rule is applicable for the event if the connection is valid
         * return true else return false else return true
         */

        if (sourceTemplate.getSimpleName().compareTo(
                    firstPort.getClass().getSimpleName()) == 0) {
            for (Class <? extends mxCell > iterableElement : targetTemplateList) {
                if (iterableElement.getSimpleName().compareTo(
                            secondPort.getClass().getSimpleName()) == 0) {
                    return true;
                }
            }
            // Nothing compatible was found ... Sorry ...
            return false;
        }

        // This rule is not applicable so we want it to be silent.
        return true;
    }
}
