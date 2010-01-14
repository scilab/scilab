/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.port;

import java.util.List;

import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
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

    private Class< ? extends mxCell> sourceTemplate;
    private List<Class< ? extends mxCell>> targetTemplateList;
    private String errorMessage;

    /**
     * Construct a new connection rule.
     * @param sourceTemplate The source ports
     * @param targetTemplate The target ports
     * @param errorMessage The associated error message.
     */
    public PortCheck(Class< ? extends mxCell> sourceTemplate, List<Class< ? extends mxCell>> targetTemplate, String errorMessage) {
	// We completely override mxMultiplicity
	super(true, null, null, null, 0, null, null, null, null, false);
	this.sourceTemplate = sourceTemplate;
	this.targetTemplateList = targetTemplate;
	this.errorMessage = errorMessage;
	this.max = "n";

    }

	/**
	 * Special case used in {@link XcosDiagram#checkMultiplicities}. In this
	 * case, source is already connected.
	 * 
	 * @param source The source port
	 * @param target The target port
	 * @param sourceOut Number of connections at the source port
	 * @param targetIn Number of connections at the target port
	 * @return An error message or null when the link is valid
	 */
    public String checkDrawLink(Object source, Object target, int sourceOut, int targetIn) {
	// maybe there is a better way to check this
	if (sourceOut > 1 || targetIn > 0) {
	    if (errorMessage.compareTo(XcosMessages.LINK_ERROR_ALREADY_CONNECTED) == 0) {
		return XcosMessages.LINK_ERROR_ALREADY_CONNECTED;
	    }
	    return null;
	}

	if (isTypeCompatible(source, target)) { return null; }
	
	return errorMessage;
    }
    
    
    /**
     * Check for the rule.
     * @param graph the graph
     * @param edge the edge
     * @param source the start
     * @param target the end
     * @param sourceOut number of connections at source.
     * @param targetIn number of connections at target
     * @return An error message or null if the link is valid
     * 
     * @see com.mxgraph.view.mxMultiplicity#check(com.mxgraph.view.mxGraph, java.lang.Object, java.lang.Object, java.lang.Object, int, int)
     */
    @Override
    public String check(mxGraph graph, Object edge, Object source, Object target, int sourceOut, int targetIn)
    {
	// maybe there is a better way to check this
	if (sourceOut > 0 || targetIn > 0) {
	    if (errorMessage.compareTo(XcosMessages.LINK_ERROR_ALREADY_CONNECTED) == 0) {
		return XcosMessages.LINK_ERROR_ALREADY_CONNECTED;
	    }
	    return null;
	}

	if (isTypeCompatible(source, target)) { return null; }
	
	return errorMessage;
    }

    
    /**
     * Check for port compatibility.
     * 
     * @param firstPort the first port to check
     * @param secondPort the second port to check
     * @return true when compatible, false otherwise
     */
    private boolean isTypeCompatible(Object firstPort, Object secondPort) {
		/* if the rule is applicable for the event
		 *     if the connection is valid
		 *         return true
		 *     else
		 *         return false
		 * else
		 *     return true
		 */
    	
	if (sourceTemplate.getSimpleName().compareTo(firstPort.getClass().getSimpleName()) == 0) {
	    for (Class< ? extends mxCell> iterableElement : targetTemplateList) {
		if (iterableElement.getSimpleName().compareTo(secondPort.getClass().getSimpleName()) == 0) {
		    return true;
		}
	    }
	    //Nothing compatible was found ... Sorry ...
	    return false;
	}

	// This rule is not applicable so we want it to be silent.
	return true;
    }
}
