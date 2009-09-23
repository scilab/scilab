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

package org.scilab.modules.xcos.block.ramp;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

public class RampBlock extends BasicBlock {
    /**
     * Default constructor
     */
    public RampBlock() {
	super("Ramp");
	
	OutputPort outputPort = new ExplicitOutputPort();
	outputPort.setDataLines(1); // model.out = 1
	addPort(outputPort);
	
	setInterfaceFunctionName("RAMP");
	
	setSimulationFunctionName("ramp"); // model.sim = list("ramp", 4)
	setSimulationFunctionType(SimulationFunctionType.C_OR_FORTRAN);
	
	getRealParameters().add(0.0); // model.rpar = [0;0;0]
	getRealParameters().add(0.0);
	getRealParameters().add(0.0);
	
	setDependsOnU(false);  // model.dep_ut=[%f %t]
	setDependsOnT(true);
    }

}
