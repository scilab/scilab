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

import org.scilab.modules.xcos.block.BasicBlock;

import edu.tum.cs.simulink.model.SimulinkBlock;

public class BlockSpecificElement {
	PatternElement patternElement;
	
	public BlockSpecificElement() {
		patternElement = new PatternElement();
	}
	
	public BasicBlock decode(SimulinkBlock from ,BasicBlock into) {
		
		
		validate();
		patternElement.printPattern();
		
		/*
		 * fill the data
		 */
		//findCorrespondingParameter(string simulinkParameter);
		
		return into;
	}

	private void validate() {
		// TODO Auto-generated method stub
		
	}
}
