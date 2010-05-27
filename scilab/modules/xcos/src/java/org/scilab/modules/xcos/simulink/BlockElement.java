package org.scilab.modules.xcos.simulink;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.simulink.InputPortElement;
import org.scilab.modules.xcos.simulink.OutputPortElement;

import com.mxgraph.model.mxCell;

import edu.tum.cs.commons.collections.UnmodifiableIterator;
import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkInPort;
import edu.tum.cs.simulink.model.SimulinkOutPort;

public class BlockElement extends AbstractElement<BasicBlock> {
	private SimulinkBlock base;
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
		
		try {
			Logger.toFile("From " + base.getName() + ":");
			Logger.toFile(base.getParameterNames().toString());
		} catch(IOException e1) {
			LogFactory.getLog(BlockElement.class).error(e1);
		}
		/*
		 * TODO: SimulinkBlock decoding, parameters etc.
		 */
		decodeParams(block);
		
		return block;
	}

	private void decodeParams(BasicBlock block) {
		// TODO Auto-generated method stub
		
		/*
		 * Allocate and setup ports
		 */
		OutputPortElement outElement = new OutputPortElement(base);
		UnmodifiableIterator<SimulinkOutPort> portOutIter = base.getOutPorts().iterator();
	    while(portOutIter.hasNext()) {
	    	//block.addPort(
	    	outElement.decode(portOutIter.next(), null);
	    }
		
		InputPortElement inElement = new InputPortElement(base);
		UnmodifiableIterator<SimulinkInPort> portInIter = base.getInPorts().iterator();
	    while(portInIter.hasNext()) {
	    	//block.addPort(
	    	inElement.decode(portInIter.next(), null);
	    }
		
		decodeSubBlocks(block);
	}

	private void decodeSubBlocks(BasicBlock block) {
		/*
		 * TODO: remember about getting subsystems to work (below)
		 * 		 add also everything about ports and lines to subsystems
		 */
		double minimalYaxisValue = 0.0;
		int i=0;
		
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
				block.getParentDiagram().addCell(cell); //not that way for sure
			}
			i++;
		}
		
	}

	public boolean canDecode(SimulinkBlock data) {
		// TODO Auto-generated method stub
		return true;
	}
}
