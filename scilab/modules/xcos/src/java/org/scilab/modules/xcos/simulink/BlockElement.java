/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Jerzy Zagorski
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.simulink;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.simulink.BlockGraphicElement;
import org.scilab.modules.xcos.simulink.InputPortElement;
import org.scilab.modules.xcos.simulink.OutputPortElement;

import com.mxgraph.model.mxCell;

import edu.tum.cs.commons.collections.UnmodifiableIterator;
import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkInPort;
import edu.tum.cs.simulink.model.SimulinkOutPort;

public class BlockElement extends AbstractElement<BasicBlock> {
	
	private SimulinkBlock base;
	private BlockGraphicElement graphicElement = new BlockGraphicElement();
	private BlockSpecificElement specificElement = new BlockSpecificElement();
	private BlockModelElement modelElement = new BlockModelElement();
	private static final Log LOG = LogFactory.getLog(BlockElement.class);
	
	/** Map from index to blocks */
	private final Map<Integer, BasicBlock> blocks;
	/**
	 * Default constructor
	 */
	public BlockElement() {
		blocks = new HashMap<Integer, BasicBlock>();
	}
	
	/* 
	 * here there will be some neccessery functions, checking if import is possible etc.
	 */
	
	public BasicBlock decode(SimulinkBlock from, BasicBlock into) {
		BasicBlock block = into;
		base = from;
		if (block == null) {
			block = BlockFactory.createBlock(from.getId());
		}
		
		/*
		 * TODO: SimulinkBlock decoding, parameters etc.
		 */
		decodeParams(block);
		
		return block;
	}

	private void decodeParams(BasicBlock block) {
		// TODO Auto-generated method stub
		
		OutputPortElement outElement = new OutputPortElement(base);
		UnmodifiableIterator<SimulinkOutPort> portOutIter = base.getOutPorts().iterator();
		while(portOutIter.hasNext()) {
			//TODO: block.addPort(outElement.decode(portOutIter.next(), null);
			//outElement.decode(portOutIter.next(), null);
			portOutIter.next();
		}
		
		InputPortElement inElement = new InputPortElement(base);
		UnmodifiableIterator<SimulinkInPort> portInIter = base.getInPorts().iterator();
		while(portInIter.hasNext()) {
			//TODO:block.addPort(inElement.decode(portInIter.next(), null);
			//inElement.decode(portInIter.next(), null);
			portInIter.next();
		}
		/*
		 * decode graphics elements of BasicBlock
		 */
		graphicElement.decode(base, block);
		specificElement.decode(base, block);
		try {
			modelElement.decode(base, block);
		} catch(SimulinkFormatException se) {
			LogFactory.getLog(BlockElement.class).error(se);
		}
		/*
		 * recursively decode all of the Block subBlocks
		 */
		decodeSubBlocks(block);
	}

	private void decodeSubBlocks(BasicBlock block) {
		/*
		 * TODO: remember about getting subsystems to work (below)
		 * 		 add also everything about ports and lines to subsystems
		 */
		double minimalYaxisValue = 0.0;
		int i=0;
		if (LOG.isTraceEnabled()) {
			LOG.trace("From " + base.getName() + " subblocks:");
			LOG.trace(base.getSubBlocks().toString());
		}
		UnmodifiableIterator<SimulinkBlock> blockIter = base.getSubBlocks().iterator();
		while(blockIter.hasNext()) {
			SimulinkBlock data = blockIter.next();
			Object cell = null;
			
			if (this.canDecode(data)) {
				BasicBlock subBlock = this.decode(data, null);
				blocks.put(i, subBlock);
				cell = subBlock;
				
				minimalYaxisValue = Math.min(minimalYaxisValue, ((mxCell) cell).getGeometry().getY());
			} 
			
			if (cell != null) {
				// FIXME: How to create subsystem in existing block?
				//block.getParentDiagram().addCell(cell); //not that way for sure
			}
			i++;
		}
		
	}

	public boolean canDecode(SimulinkBlock data) {
		// TODO Auto-generated method stub
		return true;
	}
}
