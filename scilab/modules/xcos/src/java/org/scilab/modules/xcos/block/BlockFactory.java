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

import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

/**
 * Ease the creation of blocks
 */
public final class BlockFactory {

	/**
	 * List the specific block interface function name.
	 * 
	 * @checkstyle DAC: As this is the constructor for all the block classes,
	 *             this class is very coupled with *Block classes
	 */
	private enum BlockInterFunction {
		TEXT_f(new TextBlock()),
		SUPER_f(new SuperBlock()),
		DSUPER(new SuperBlock() { { mask(); } }),
		CONST_m(new ConstBlock()),
		CONST(new ConstBlock()),
		CONST_f(new ConstBlock()),
		AFFICH_m(new AfficheBlock()),
		AFFICH_f(new AfficheBlock()),
		GAINBLK_f(new GainBlock()),
		GAINBLK(new GainBlock()),
		GAIN_f(new GainBlock()),
		IN_f(new ExplicitInBlock()),
		OUT_f(new ExplicitOutBlock()),
		INIMPL_f(new ImplicitInBlock()),
		OUTIMPL_f(new ImplicitOutBlock()),
		CLKINV_f(new EventInBlock()),
		/* TODO: What about CLKIN_F ? */
		CLKOUTV_f(new EventOutBlock()),
		CLKOUT_f(new EventOutBlock()),
		SPLIT(new SplitBlock()),
		IMPSPLIT_f(new SplitBlock()),
		CLKSPLIT_f(new SplitBlock());
		
		private BasicBlock block;
		/**
		 * Default constructor
		 * @param block The reference instance
		 */
		private BlockInterFunction(BasicBlock block) {
			this.block = block;
			block.setValue(this.name());
		}
		
		/**
		 * Create a block instance
		 * @return The new block instance
		 */
		public BasicBlock createInstance() {
			BasicBlock clone = null;
			
			if (block != null) {
				clone = (BasicBlock) BlockFactory.createClone(block);
			}
			return clone;
		}
	}
	
	/** Default singleton constructor */
	private BlockFactory() {
		// This class is a static singleton
	}

	/**
	 * Instanciate a new block with the specified interface function name.
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
