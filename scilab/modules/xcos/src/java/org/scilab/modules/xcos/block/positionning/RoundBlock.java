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
import org.scilab.modules.xcos.block.listener.ProdPortLabelingListener;
import org.scilab.modules.xcos.block.listener.SumPortLabelingListener;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.output.OutputPort;

/**
 * Implement a round block with inputs spread around the block.
 */
public class RoundBlock extends BasicBlock {
	private static final int SIDE_NUMBER = Orientation.values().length;
	
	/**
	 * Default constructor
	 */
	public RoundBlock() {
		super();
	}
	
	/**
	 * Set default values
	 * @param interFunction the interfunction (label) string
	 */
	public RoundBlock(String interFunction) {
		super(interFunction);
		
		if (interFunction.equals("SUM_f")) {
			parametersPCS.addPropertyChangeListener("integerParameters", SumPortLabelingListener.getInstance());
		} else if (interFunction.equals("PROD_f")) {
			parametersPCS.addPropertyChangeListener("realParameters", ProdPortLabelingListener.getInstance());
		}
	}
	
	/**
	 * Calculate current port position on the block and add it.
	 * @param port the port to add
	 * @see org.scilab.modules.xcos.block.BasicBlock#addPort(org.scilab.modules.xcos.port.BasicPort)
	 */
	@Override
	public void addPort(BasicPort port) {
		/*
		 * Any output port keep its orientation.
		 */
		if (port instanceof OutputPort || port instanceof CommandPort) {
			super.addPort(port);
			return;
		}
		
		/*
		 * The other ones are placed around the block.
		 */
		final int def = port.getOrientation().ordinal() - 1;
		final int side = getChildCount();
		
		final int rotatedSide = (side + def + SIDE_NUMBER) % SIDE_NUMBER;
		
		final Orientation current = Orientation.values()[rotatedSide];
		port.setOrientation(current);
		
		super.addPort(port);
	}
}
