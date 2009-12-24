package org.scilab.modules.xcos.block;

import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

public final class BlockFactory {

	public static BasicBlock createBlock(String label) {
		if (label.compareTo("TEXT_f") == 0) {
			return new TextBlock(label);
		}
		if (label.compareTo("SUPER_f") == 0) {
			return new SuperBlock(label);
		}
		if (label.compareTo("DSUPER") == 0) {
			return new SuperBlock(label, true);
		}
		if (label.compareTo("CONST_m") == 0 || label.compareTo("CONST") == 0
				|| label.compareTo("CONST_f") == 0) {
			return new ConstBlock(label);
		}
		if (label.compareTo("AFFICH_m") == 0
				|| label.compareTo("AFFICH_f") == 0) {
			return new AfficheBlock(label);
		}
		if (label.compareTo("GAINBLK_f") == 0
				|| label.compareTo("GAINBLK") == 0
				|| label.compareTo("GAIN_f") == 0) {
			return new GainBlock(label);
		}
		if (label.compareTo("IN_f") == 0) {
			return new ExplicitInBlock(label);
		}
		if (label.compareTo("OUT_f") == 0) {
			return new ExplicitOutBlock(label);
		}
		if (label.compareTo("INIMPL_f") == 0) {
			return new ImplicitInBlock(label);
		}
		if (label.compareTo("OUTIMPL_f") == 0) {
			return new ImplicitOutBlock(label);
		}
		if (label.compareTo("CLKINV_f") == 0) {
			return new EventInBlock(label);
		}
		if (label.compareTo("CLKOUTV_f") == 0
				|| label.compareTo("CLKOUT_f") == 0) {
			return new EventOutBlock(label);
		}
		if (label.compareTo("SPLIT_f") == 0
				|| label.compareTo("IMPSPLIT_f") == 0
				|| label.compareTo("CLKSPLIT_f") == 0) {
			return new SplitBlock(label);
		} else {
			return new BasicBlock(label);
		}
	}

	/**
	 * Create a clone for block added by context menu in palette
	 * @param block The block to be cloned
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
