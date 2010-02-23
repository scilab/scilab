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
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

/**
 * The Ground block has only one specificity : it's port position.
 */
public class GroundBlock extends BasicBlock {
	
	/** Default constructor */
	public GroundBlock() {
		super();
	}
	
	/**
	 * Set the orientation before calling parent method.
	 * @param port the port to add.
	 */
	@Override
	public void addPort(BasicPort port) {
		port.setOrientation(Orientation.NORTH);
		super.addPort(port);
	}
}
