/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.sinusoid;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

public class SinusoidBlock extends BasicBlock {
    /**
     * Default constructor
     */
    public SinusoidBlock() {
	super("Sinusoid Generator", "Sinusoid");
	
	OutputPort outputPort = new ExplicitOutputPort();
	outputPort.setDataLines(1); // model.out = 1
	addPort(outputPort);
	
	setInterfaceFunctionName("GENSIN_f");
	
	setSimulationFunctionName("gensin"); // model.sim = "gensin"
	
	getRealParameters().add(1.0); // model.rpar = [1;1;0]
	getRealParameters().add(1.0);
	getRealParameters().add(0.0);
	
	setDependsOnU(false);  // model.dep_ut=[%f %t]
	setDependsOnT(true);
	
	 // exprs=[string(rpar(1));string(rpar(2));string(rpar(3))]
	getExprs().add("1");
	getExprs().add("1");
	getExprs().add("0");
    }

}
