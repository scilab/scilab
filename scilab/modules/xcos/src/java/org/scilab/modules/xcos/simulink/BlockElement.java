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

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;

import com.mxgraph.model.mxCell;

import edu.tum.cs.commons.collections.UnmodifiableIterator;
import edu.tum.cs.simulink.model.SimulinkAnnotation;
import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkInPort;
import edu.tum.cs.simulink.model.SimulinkLine;
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
	/**
	 * Current block ordering, the ordering change on each {@link BlockElement}
	 * instance so be careful when allocated a new {@link BlockElement}.
	 */
	private int ordering;
	
	/* 
	 * here there will be some neccessery functions, checking if import is possible etc.
	 */
	
	public BasicBlock decode(SimulinkBlock from, BasicBlock into, XcosDiagram diag) {
		BasicBlock block = into;
		base = from;
		if (block == null) {
			block = BlockFactory.createBlock(modelElement.getInterFunctionName(base));
			LOG.trace(modelElement.getInterFunctionName(base));
		}
		block.setParentDiagram(diag);
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
		OutputPort[] outPortTempTable= new OutputPort[base.getOutPorts().size() + 1];
		while(portOutIter.hasNext()) {
			OutputPort portAdd = outElement.decode(portOutIter.next(), null);
			outPortTempTable[portAdd.getOrdering()] = portAdd;
		}
		for(int i = 1 ; i < outPortTempTable.length ; i++){
			block.addPort(outPortTempTable[i]);
		}
		
		InputPortElement inElement = new InputPortElement(base);
		UnmodifiableIterator<SimulinkInPort> portInIter = base.getInPorts().iterator();
		InputPort[] inPortTempTable= new InputPort[base.getInPorts().size() + 1];
		while(portInIter.hasNext()) {
			InputPort portAdd = inElement.decode(portInIter.next(), null);
			if (LOG.isDebugEnabled()) {
				LOG.debug(inPortTempTable.length);
				LOG.debug(portAdd.getOrdering());
				LOG.debug(portAdd.getId().toString());
			}
			inPortTempTable[portAdd.getOrdering()] = portAdd;
		}
		for(int i = 1 ; i < inPortTempTable.length ; i++){
			block.addPort(inPortTempTable[i]);
		}
		/*
		 * Add control and command ports
		 */
		for(int i=0 ; i< modelElement.getControlPorts(base) ; i++ ){
			ControlPort portAdd = inElement.decodeControlPort();
			block.addPort(portAdd);
		}
		
		/*
		 * Add control and command ports
		 */
		for(int i=0 ; i< modelElement.getCommandPorts(base) ; i++ ){
			CommandPort portAdd = outElement.decodeCommandPort();
			block.addPort(portAdd);
		}
		/*
		 * decode graphics elements of BasicBlock
		 */
		graphicElement.decode(base, block);
		
		try {
			modelElement.decode(base, block);
		} catch(SimulinkFormatException se) {
			LogFactory.getLog(BlockElement.class).error(se);
		}
		
		block.generateId();
		/*
		 * Set state dependent informations.
		 */
		block.setOrdering(ordering);
		ordering++;
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
		if(block instanceof SuperBlock){
			SuperBlock sBlock = (SuperBlock) block;
			SuperBlockDiagram child = new SuperBlockDiagram();
			DiagramElement diagram = new DiagramElement();
			child.setContainer(sBlock);
			child.installListeners();
			try {
			diagram.decode(base, child);
			} catch(SimulinkFormatException e1) {
				LogFactory.getLog(ImportMdl.class).error(e1);
			}
			sBlock.setChild(child);
		}
	}

	public boolean canDecode(SimulinkBlock data) {
		// TODO Auto-generated method stub
		return true;
	}
}
