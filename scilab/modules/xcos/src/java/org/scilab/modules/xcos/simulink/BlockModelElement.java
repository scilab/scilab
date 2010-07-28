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

import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;

import edu.tum.cs.simulink.model.SimulinkBlock;

public class BlockModelElement{

	private PatternElement patternElement;
	SimulinkBlock data;
	
	public BasicBlock decode(SimulinkBlock from, BasicBlock into) throws SimulinkFormatException {
	if (into == null) {
		throw new IllegalArgumentException();
	}
	validate();
	/*
	 * mutable field for passing to submethods
	 */
	BasicBlock base = into;
	data = from;
	/*
	 * initialize patterns for particular block type
	 */
	patternElement = new PatternElement(data.getParameter("BlockType"));
	/*
	 * fill the data
	 */
	fillSimulationFunction(base);
	fillParameters(base);

	return into;
	}
	
	private void fillSimulationFunction(BasicBlock base) {
		// TODO Auto-generated method stub
		base.setSimulationFunctionName(patternElement.decodeFunctionName(data));
		if(patternElement.haveFunctionType(data)){
			base.setSimulationFunctionType(patternElement.decodeFunctionType(data));
		}
	}

	private void fillParameters(BasicBlock base) {
		// TODO Auto-generated method stub
		// Strings including formal expressions used in the dialog box of the block. 
		base.setExprs(patternElement.decodeExprs(data));
		// state - vector conaining initial values of continous-time state
		base.setState((ScilabType)patternElement.decodeState(data));

		// dstate - vector conaining initial values of discrete-time state
		base.setDState(patternElement.decodeDState(data));

		// odstate - List containing initial values of objects state. 
		base.setODState(patternElement.decodeODState(data));

		// rpar - The vector of floating point block parameters. 
		base.setRealParameters((ScilabType)patternElement.decodeRealParameters(data));

		// ipar - The vector of integer block parameters
		base.setIntegerParameters((ScilabType)patternElement.decodeIntegerParameters(data));

		// opar - List of objects block parameters. Must be list() if no objects parameters.
		base.setObjectsParameters((ScilabType)patternElement.decodeObjectsParameters(data));

		// blocktype - Character that can be set to 'c' or 'd' indifferently for standard blocks. 'x' is used if we want to force the computational function to be called during the simulation phase even if the block does not contribute to computation of the state derivative. 
		// l', 'm' and 's' are reserved. Not to be used. 
		base.setBlockType(patternElement.decodeBlockType(data));
		
		// depends on time
		base.setDependsOnT(patternElement.decodeDependsOnT(data));

		// nzcross - Number of zero-crossing surfaces. 
		base.setNbZerosCrossing(patternElement.decodeNbZerosCrossing(data));

		// nmode - Length of the mode register. Note that this gives the size of the vector mode and not the total number of modes in which a block can operate in. Suppose a block has 3 modes and each mode can take two values, then the block can have up to 2^3=8 modes. 
		base.setNmode(patternElement.decodeNmode(data));

		// equation Used in case of implicit blocks. 
		//Data structure of type modelica which contains modelica code description if any. That list contains four entries :
		base.setEquations((ScilabType)patternElement.decodeEquations(data));
		
		
		// firing
		// Vector of initial event firing times of size equal to the number of activation output ports (see evout). It contains output initial event dates (Events generated before any input event arises). Negative values stands for no initial event on the corresponding port.
		base.setInterfaceFunctionName(patternElement.decodeInterfaceFunctionName(data));
	}
	
	public String getInterFunctionName(SimulinkBlock from){
		//FIXME: shouldn't have to multiple initialize
		patternElement = new PatternElement(from.getParameter("BlockType"));
		return patternElement.decodeInterfaceFunctionName(from);
	}
	
	public int getControlPorts(SimulinkBlock from){
		//FIXME: shouldn't have to multiple initialize
		patternElement = new PatternElement(from.getParameter("BlockType"));
		return patternElement.decodeControlPorts(from);
	}

	public int getCommandPorts(SimulinkBlock from){
		//FIXME: shouldn't have to multiple initialize
		patternElement = new PatternElement(from.getParameter("BlockType"));
		return patternElement.decodeCommandPorts(from);
	}
	
	private void validate() {
		// TODO Auto-generated method stub
		
	}
}
