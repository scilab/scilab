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

import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosEvent;

import com.mxgraph.util.mxEventObject;

public class ImplicitInBlock extends BasicBlock {

	public ImplicitInBlock() {
		super();
		setVertex(false);
		setVisible(false);
	}

	public ImplicitInBlock(String label) {
		super(label);
		setInterfaceFunctionName("INIMPL_f");
		setValue("1");
	}

	public void setExprs(ScilabType exprs) {
		super.setExprs(exprs);
		setValue(((ScilabString) getExprs()).getData()[0][0]);
	}

	public void updateBlockSettings(BasicBlock modifiedBlock) {

		String oldValue = (String)getValue();
   		super.updateBlockSettings(modifiedBlock);
		String newValue = (String)getValue();
   		
		if(oldValue.compareTo(newValue) != 0){
			getParentDiagram().fireEvent(XcosEvent.IN_IMPLICIT_VALUE_UPDATED, new mxEventObject(new Object[]{oldValue,newValue}));
		}
   		
	}
}
