/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
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
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.input.InputPort;

/**
 * Implement a round block with inputs spread around the block.
 */
public class RoundBlock extends BasicBlock {
	private static final int SIDE_NUMBER = Orientation.values().length;
	
	/**
	 * Set default values
	 * @param interFunction the interfunction (label) string
	 */
	public RoundBlock(String interFunction) {
		super(interFunction);
	}
	
	/**
	 * Calculate current port position on the block and add it.
	 * @param port the port to add
	 * @see org.scilab.modules.xcos.block.BasicBlock#addPort(org.scilab.modules.xcos.port.input.InputPort)
	 */
	@Override
	public void addPort(InputPort port) {
		final int def = port.getOrientation().ordinal() - 1;
		final int side = getChildCount();
		
		final int rotatedSide = (side + def + SIDE_NUMBER) % SIDE_NUMBER;
		
		final Orientation current = Orientation.values()[rotatedSide];
		port.setOrientation(current);
		
		super.addPort(port);
	}
}
