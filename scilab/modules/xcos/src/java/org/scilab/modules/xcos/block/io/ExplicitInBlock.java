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

package org.scilab.modules.xcos.block.io;

import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosEvent;

import com.mxgraph.util.mxEventObject;

/**
 * @author Antoine ELIAS
 *
 */
public final class ExplicitInBlock extends ContextUpdate {

    private static final long serialVersionUID = -5872963017904352162L;

	/**
	 * Constructor
	 */
	public ExplicitInBlock() {
		super();
	}

	/**
	 * @param label block label
	 */
	protected ExplicitInBlock(String label) {
		this();
		setValue(label);
	}

	/**
	 * Initialize the block with the default values
	 */
	@Override
	protected void setDefaultValues() {
		super.setDefaultValues();
		setInterfaceFunctionName("IN_f");
		setSimulationFunctionName("input");
	}
    
    public void setExprs(ScilabType exprs) {
	super.setExprs(exprs);
    }

    public void updateBlockSettings(BasicBlock modifiedBlock) {

	double oldValue = ((ScilabDouble) getIntegerParameters()).getRealPart()[0][0];
	super.updateBlockSettings(modifiedBlock);
	double newValue = ((ScilabDouble) getIntegerParameters()).getRealPart()[0][0];

	if (oldValue != newValue) {
	    getParentDiagram().fireEvent(new mxEventObject(XcosEvent.IN_EXPLICIT_VALUE_UPDATED, XcosConstants.EVENT_CHANGE_OLD,
		    oldValue, XcosConstants.EVENT_CHANGE_NEW, newValue));
	}
    }

    public void setIntegerParameters(ScilabType integerParameters) {
	super.setIntegerParameters(integerParameters);
	setValue((int) ((ScilabDouble) getIntegerParameters()).getRealPart()[0][0]);
	if (getParentDiagram() != null) {
	    getParentDiagram().refresh();
	}
    }

}
