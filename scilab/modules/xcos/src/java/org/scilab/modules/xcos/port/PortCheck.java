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

import java.util.Collection;
import java.util.List;

import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxMultiplicity;

public class PortCheck extends mxMultiplicity {

    private Class<? extends mxCell> sourceTemplate = null;
    private List<Class<? extends mxCell>> targetTemplateList = null;
    private String errorMessage = null;

    public PortCheck(boolean source, String type, String attr, String value,
	    int min, String max, Collection<String> validNeighbors, String countError,
	    String typeError, boolean validNeighborsAllowed) {
	super(source, type, attr, value, min, max, validNeighbors, countError,
		typeError, validNeighborsAllowed); 
    }
    
    public PortCheck(Class<? extends mxCell> sourceTemplate, List<Class<? extends mxCell>> targetTemplate, String errorMessage) {
	// We complitely override mxMultiplicity
	super(true, null, null, null, 0, null, null, null, null, false);
	this.sourceTemplate = sourceTemplate;
	this.targetTemplateList = targetTemplate;
	this.errorMessage = errorMessage;
	this.max = "n";

    }

    //Special case for drawLink 
    //In this case, source are already connected.
    public String checkDrawLink(mxGraph graph, Object edge, Object source, Object target, int sourceOut, int targetIn) {
	// maybe there is a better way to check this
	if(sourceOut > 1 || targetIn > 0) {
	    if(errorMessage.compareTo(XcosMessages.LINK_ERROR_ALREADY_CONNECTED) == 0) {
		return XcosMessages.LINK_ERROR_ALREADY_CONNECTED;
	    }
	    return null;
	}

	if (isTypeCompatible(source, target)) { return null; }
	
	return errorMessage;
    }
    
    public String check(mxGraph graph, Object edge, Object source, Object target, int sourceOut, int targetIn)
    {
	// maybe there is a better way to check this
	if(sourceOut > 0 || targetIn > 0) {
	    if(errorMessage.compareTo(XcosMessages.LINK_ERROR_ALREADY_CONNECTED) == 0) {
		return XcosMessages.LINK_ERROR_ALREADY_CONNECTED;
	    }
	    return null;
	}

	if (isTypeCompatible(source, target)) { return null; }
	
	return errorMessage;
    }

    private boolean isTypeCompatible(Object firstPort, Object secondPort) {

	if (sourceTemplate.getSimpleName().compareTo(firstPort.getClass().getSimpleName()) == 0) {
	    for (Class<? extends mxCell> iterable_element : targetTemplateList) {
		if (iterable_element.getSimpleName().compareTo(secondPort.getClass().getSimpleName()) == 0) {
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
