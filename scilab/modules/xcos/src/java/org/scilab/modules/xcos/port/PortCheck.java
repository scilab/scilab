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

import com.mxgraph.model.mxCell;
import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxMultiplicity;

public class PortCheck extends mxMultiplicity {

    private mxCell sourceTemplate = null;
    private mxCell[] targetTemplate = null;
    private String errorMessage = null;

    public PortCheck(boolean source, String type, String attr, String value,
	    int min, String max, Collection validNeighbors, String countError,
	    String typeError, boolean validNeighborsAllowed) {
	super(source, type, attr, value, min, max, validNeighbors, countError,
		typeError, validNeighborsAllowed); 
    }

    public PortCheck(mxCell sourceTemplate, mxCell[] targetTemplate, String errorMessage) {
	// We complitely override mxMultiplicity
	super(true, null, null, null, 0, null, null, null, null, false);
	this.sourceTemplate = sourceTemplate;
	this.targetTemplate = targetTemplate;
	this.errorMessage = errorMessage;
	this.max = "n";

    }

    public String check(mxGraph graph, Object edge, Object source, Object target, int sourceOut, int targetIn)
    {	
	if (isTypeCompatible(source, target)) { return null; }
	return errorMessage;
    }

    private boolean isTypeCompatible(Object firstPort, Object secondPort) {

	if (sourceTemplate.getClass().getSimpleName().compareTo(firstPort.getClass().getSimpleName()) == 0) {
	    if (firstPort instanceof BasicPort) {
		BasicPort port = (BasicPort) firstPort;
		if (port.getEdgeCount() > 0) {
		    return false;
		}
	    }

	    if (secondPort instanceof BasicPort) {
		BasicPort port = (BasicPort) secondPort;
		if (port.getEdgeCount() > 0) {
		    System.err.println("Port already connected !!");
		    return false;
		}
	    }

	    for (int i = 0; i < targetTemplate.length; ++i) {
		if (targetTemplate[i].getClass().getSimpleName().compareTo(secondPort.getClass().getSimpleName()) == 0) {
		    // We found something compatible !!
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
