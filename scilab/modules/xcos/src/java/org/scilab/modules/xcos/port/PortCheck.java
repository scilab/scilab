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

import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxMultiplicity;

public class PortCheck extends mxMultiplicity {

    private BasicPort sourceTemplate = null;
    private BasicPort targetTemplate = null;
    private String errorMessage = null;

    public PortCheck(boolean source, String type, String attr, String value,
	    int min, String max, Collection validNeighbors, String countError,
	    String typeError, boolean validNeighborsAllowed) {
	super(source, type, attr, value, min, max, validNeighbors, countError,
		typeError, validNeighborsAllowed); 
    }

    public PortCheck(BasicPort sourceTemplate, BasicPort targetTemplate, String errorMessage) {
	// We complitely override mxMultiplicity
	super(true, null, null, null, 0, null, null, null, null, false);
	this.sourceTemplate = sourceTemplate;
	this.targetTemplate = targetTemplate;
	this.errorMessage = errorMessage;
	this.max = "n";

    }

    public String check(mxGraph graph, Object edge, Object source, Object target, int sourceOut, int targetIn)
    {
	//System.err.println("errorMessage : "+errorMessage);
	//System.err.println("Calling check : "+source.getClass().getSimpleName()+" -> "+target.getClass().getSimpleName());
	//System.err.println("Versus : "+sourceTemplate.getClass().getSimpleName()+" -> "+targetTemplate.getClass().getSimpleName());
	if (isTypeCompatible(source, target)) { return null; }

	return errorMessage;
    }

    //    public boolean checkType(mxGraph graph, Object value, String type)
    //    {
    //	System.err.println("Calling checkType (3)");
    //	return false;
    //    }
    //
    //    public boolean checkType(mxGraph graph, Object value, String type,
    //	    String attr, String attrValue)
    //    {
    //	System.err.println("Calling checktype(5)");
    //	return false;
    //    }
    //    public boolean isUnlimited() {
    //	System.err.println("Calling isunlimited()");
    //	return false;
    //    }
    //    public int getMaxValue() {
    //	System.err.println("Calling getMaxValue()");
    //	return 2;
    //    }

    private boolean isTypeCompatible(Object firstPort, Object secondPort) {
	// Source template is the same as the one given.
	if (this.sourceTemplate.getClass().getSimpleName().compareTo(firstPort.getClass().getSimpleName()) == 0) {
	    if (targetTemplate.getClass().getSimpleName().compareTo(secondPort.getClass().getSimpleName()) != 0) {
		return false;
	    }
	    else {
		// This rule is not applicable so we want it to be silent.
		return true;
	    }
	}

	// This rule is not applicable so we want it to be silent.
	return true;
    }

}
