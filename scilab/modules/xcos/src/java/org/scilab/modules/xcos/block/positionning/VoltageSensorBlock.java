/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.positionning;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.scicos.BasicBlockInfo;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;

import com.mxgraph.model.mxICell;

/**
 * The VoltageSensor block has only one specificity : it's port position.
 */
public class VoltageSensorBlock extends BasicBlock {
	
	/** Default constructor */
	public VoltageSensorBlock() {
		super();
	}
	
	/**
	 * Set the orientation before calling parent method.
	 * @param child the port to add.
	 * @param index the port index
	 */
	@Override
	public mxICell insert(mxICell child, int index) {
		if (child instanceof OutputPort) {
			final OutputPort port = (OutputPort) child;
			
			// Only orientate the first output port
			if (port.getOrdering() == 1) {
				port.setOrientation(Orientation.SOUTH);
			}
		}
		
		return super.insert(child, index);
	}
}
