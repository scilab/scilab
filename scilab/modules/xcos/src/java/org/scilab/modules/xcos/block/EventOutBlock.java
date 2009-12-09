/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block;

import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.utils.XcosEvent;

import com.mxgraph.util.mxEventObject;

public final class EventOutBlock extends ContextUpdate {

    private static final long serialVersionUID = 6585655980747217421L;

    public EventOutBlock() {
	super();
	setVertex(false);
	setVisible(false);
    }

    public EventOutBlock(String label) {
	super(label);
	setInterfaceFunctionName("CLKOUTV_f");
	setSimulationFunctionName("output");
	setNbZerosCrossing(new ScilabDouble(0));
	setNmode(new ScilabDouble(0));
	setODState(new ScilabList());
	setValue(1);
    }

    public void setExprs(ScilabType exprs) {
	super.setExprs(exprs);
	//setValue(((ScilabString) getExprs()).getData()[0][0]);
    }

    public void updateBlockSettings(BasicBlock modifiedBlock) {

	double oldValue = ((ScilabDouble)getIntegerParameters()).getRealPart()[0][0];
	super.updateBlockSettings(modifiedBlock);
	double newValue = ((ScilabDouble)getIntegerParameters()).getRealPart()[0][0];

	if(oldValue != newValue){
	    getParentDiagram().fireEvent(XcosEvent.OUT_EVENT_VALUE_UPDATED, new mxEventObject(new Object[]{oldValue,newValue}));
	}
    }

    public void setIntegerParameters(ScilabType integerParameters) {
	super.setIntegerParameters(integerParameters);
	setValue((int)((ScilabDouble)getIntegerParameters()).getRealPart()[0][0]);
    }
}
