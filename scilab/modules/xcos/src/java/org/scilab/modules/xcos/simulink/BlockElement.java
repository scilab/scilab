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
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

import edu.tum.cs.commons.collections.UnmodifiableIterator;
import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkInPort;
import edu.tum.cs.simulink.model.SimulinkOutPort;

public class BlockElement extends AbstractElement<BasicBlock> {

	private SimulinkBlock base;
	private BlockGraphicElement graphicElement = new BlockGraphicElement();
	/*
	 * blockSpecificElement is for trace and debugging
	 */
	private TraceElement traceElement;
	private BlockModelElement modelElement = new BlockModelElement();
	private static final Log LOG = LogFactory.getLog(BlockElement.class);

	// FIXME: check if those functions are necessary
	/** 
	 * Map from index to blocks 
	 */
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

	/**
	 * @param from - source SimulinkBlock
	 * @param into - BasicBlock to translate SimulinkBlock into
	 * @param diag - parent diagram of decoding block
	 * @return
	 */
	public BasicBlock decode(SimulinkBlock from, BasicBlock into, XcosDiagram diag, TraceElement trace) {

		BasicBlock block = into;
		base = from;
		traceElement = trace;

		try {
			if(LOG.isTraceEnabled()) {
				LOG.trace("== Decoding block: " + from.getName() + " of type: " + modelElement.getInterFunctionName(base) + ".==");
			}

			if (block == null) {
				block = BlockFactory.createBlock(modelElement.getInterFunctionName(base));
			}
			block.setParentDiagram(diag);

			decodeParams(block);
		} catch (PatternBindingException pbe) {
			// TODO: some handling here, probably add blocks patterns unable to bind to some list
			LOG.error(pbe);
		}

		return block;
	}
	/**
	 * Parent function for decoding ports/graphics/model parameters of BasicBlock
	 * @param block to decode parameters into
	 */
	private void decodeParams(BasicBlock block) {

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
			inPortTempTable[portAdd.getOrdering()] = portAdd;
		}
		for(int i = 1 ; i < inPortTempTable.length ; i++){
			block.addPort(inPortTempTable[i]);
		}
		/*
		 * Add control and command ports
		 */
		try {
			for(int i=0 ; i< modelElement.getControlPorts(base) ; i++ ){
				ControlPort portAdd = inElement.decodeControlPort();
				block.addPort(portAdd);
			}
		} catch (PatternBindingException pbe) {
			// TODO: some handling here, probably add blocks patterns unable to bind to some list
			LOG.error(pbe);
		}

		/*
		 * Add control and command ports
		 */
		try {
			for(int i=0 ; i< modelElement.getCommandPorts(base) ; i++ ){
				CommandPort portAdd = outElement.decodeCommandPort();
				block.addPort(portAdd);
			}
		} catch (PatternBindingException pbe) {
			// TODO: some handling here, probably add blocks patterns unable to bind to some list
			LOG.error(pbe);
		}
		/*
		 * decode graphics elements of BasicBlock
		 */
		graphicElement.decode(base, block);

		try {
			modelElement.decode(base, block, traceElement);
		} catch(SimulinkFormatException se) {
			LOG.error(se);
		} catch (PatternBindingException pbe) {
			// TODO: some handling here, probably add blocks patterns unable to bind to some list
			LOG.error(pbe);
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
	/**
	 * Function for decoding SuperBlock's SubBlocks
	 * @param block
	 */
	private void decodeSubBlocks(BasicBlock block) {
		/*
		 * If block is instance of superblock try to decode its subdiagram.
		 */
		if(block instanceof SuperBlock){
			SuperBlock sBlock = (SuperBlock) block;
			SuperBlockDiagram child = new SuperBlockDiagram();
			DiagramElement diagram = new DiagramElement();
			child.setContainer(sBlock);
			child.installListeners();
			try {
				diagram.decode(base, child, traceElement);
			} catch(SimulinkFormatException e1) {
				LogFactory.getLog(ImportMdl.class).error(e1);
			}
			sBlock.setChild(child);
		}
	}
	/**
	 * Function validating compatibility patterns of decoding block
	 * @param data
	 * @return boolean canDecode value
	 */
	public boolean canDecode(SimulinkBlock data) {
		/* TODO:
		 * If compatibility pattern exist trace description from them
		 * otherwise trace block name that compatibility pattern is missing
		 */
		//LOG.trace(arg0);
		return true;
	}
}
