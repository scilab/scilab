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

package org.scilab.modules.xcos.block;

import org.scilab.modules.xcos.block.io.EventInBlock;
import org.scilab.modules.xcos.block.io.EventOutBlock;
import org.scilab.modules.xcos.block.io.ExplicitInBlock;
import org.scilab.modules.xcos.block.io.ExplicitOutBlock;
import org.scilab.modules.xcos.block.io.ImplicitInBlock;
import org.scilab.modules.xcos.block.io.ImplicitOutBlock;
import org.scilab.modules.xcos.block.positionning.GroundBlock;
import org.scilab.modules.xcos.block.positionning.VoltageSensorBlock;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Ease the creation of blocks
 */
public final class BlockFactory {

	// DAC: As this is the constructor for all the block classes, this class is
	// very coupled with *Block classes
	// CSOFF: ClassDataAbstractionCoupling
	/**
	 * List the specific block interface function name.
	 */
	public static enum BlockInterFunction {
		TEXT_f(new TextBlock(XcosMessages.DOTS)),
		SUPER_f(new SuperBlock()),
		DSUPER(new SuperBlock(true)),
		CONST_m(new ConstBlock()),
		CONST(CONST_m.getSharedInstance()),
		CONST_f(CONST_m.getSharedInstance()),
		AFFICH_m(new AfficheBlock()),
		AFFICH_f(AFFICH_m.getSharedInstance()),
		GAINBLK_f(new GainBlock()),
		GAINBLK(GAINBLK_f.getSharedInstance()),
		GAIN_f(GAINBLK_f.getSharedInstance()),
		IN_f(new ExplicitInBlock()),
		OUT_f(new ExplicitOutBlock()),
		INIMPL_f(new ImplicitInBlock()),
		OUTIMPL_f(new ImplicitOutBlock()),
		CLKINV_f(new EventInBlock()),
		CLKOUTV_f(new EventOutBlock()),
		CLKOUT_f(CLKOUTV_f.getSharedInstance()),
		SPLIT_f(new SplitBlock()),
		IMPSPLIT_f(SPLIT_f.getSharedInstance()),
		CLKSPLIT_f(SPLIT_f.getSharedInstance()),
		Ground(new GroundBlock()),
		VoltageSensor(new VoltageSensorBlock());
		
		private BasicBlock block;
		/**
		 * Default constructor
		 * @param block The reference instance
		 */
		private BlockInterFunction(BasicBlock block) {
			this.block = block;
		}
		
		/**
		 * Create a block instance
		 * @return The new block instance
		 */
		private BasicBlock createInstance() {
			BasicBlock clone = null;
			
			if (block != null) {
				clone = (BasicBlock) BlockFactory.createClone(block);
			}
			return clone;
		}
		
		/**
		 * Get the reference shared block instance for this BlockInterFunction.
		 * @return The shared block instance
		 */
		public BasicBlock getSharedInstance() {
			return block;
		}
	}
	// CSON: ClassDataAbstractionCoupling
	
	/** Default singleton constructor */
	private BlockFactory() {
		// This class is a static singleton
	}

	/**
	 * Instantiate a new block with the specified interface function name.
	 * @param label The interface function name.
	 * @return A new instance of a block.
	 */
	public static BasicBlock createBlock(String label) {
		BasicBlock block = null;
		
		for (BlockInterFunction func : BlockInterFunction.values()) {
			if (label.compareTo(func.name()) == 0) {
				block = func.createInstance();
				break;
			}
		}
		
		// Not specific block
		if (block == null) {
			block = new BasicBlock(label);
		}
		
		return block;
	}
	
	/**
	 * Instantiate a new block with the specified interface function.
	 * @param func the interface function
	 * @return A new instance of a block. 
	 */
	public static BasicBlock createBlock(BlockInterFunction func) {
		return func.createInstance();
	}

	/**
	 * Create a clone for a block
	 * 
	 * @param block
	 *            The block to be cloned
	 * @return the clone
	 */
	public static Object createClone(BasicBlock block) {
		try {
			BasicBlock clone = (BasicBlock) block.clone();

			/* Clone children */
			for (int i = 0; i < block.getChildCount(); i++) {
				if (block.getChildAt(i) instanceof InputPort) {
					clone.addPort((InputPort) block.getChildAt(i).clone());
				} else if (block.getChildAt(i) instanceof OutputPort) {
					clone.addPort((OutputPort) block.getChildAt(i).clone());
				} else if (block.getChildAt(i) instanceof CommandPort) {
					clone.addPort((CommandPort) block.getChildAt(i).clone());
				} else if (block.getChildAt(i) instanceof ControlPort) {
					clone.addPort((ControlPort) block.getChildAt(i).clone());
				}
			}

			return clone;
		} catch (CloneNotSupportedException e) {
			e.printStackTrace();
			return null;
		}
	}
}
