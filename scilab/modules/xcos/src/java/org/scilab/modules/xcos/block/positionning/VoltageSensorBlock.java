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
import org.scilab.modules.xcos.io.BasicBlockInfo;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;

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
	 * @param port the port to add.
	 */
	@Override
	public void addPort(OutputPort port) {
		insert(port);
		int order = BasicBlockInfo.getAllOutputPorts(this, false).size();
		port.setOrdering(order);
		
		// Only orientate the first output port
		if (order == 1) {
			port.setOrientation(Orientation.SOUTH);
		} else {
			port.setOrientation(Orientation.EAST);
		}
		
		BlockPositioning.updateBlockView(this);
	}
}
