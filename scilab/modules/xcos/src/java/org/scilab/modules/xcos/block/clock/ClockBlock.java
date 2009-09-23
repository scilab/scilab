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

package org.scilab.modules.xcos.block.clock;

import java.util.ArrayList;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.command.CommandPort;

public class ClockBlock extends BasicBlock {

    /**
     * Default constructor
     */
    public ClockBlock() {
	super("Clock", "Clock");
	
	CommandPort commandPort = new CommandPort(); // model.evtout=1
	commandPort.setDataLines(1); // model.firing=-1
	commandPort.setInitialState(-1.0);
	addPort(commandPort);
	
	setSimulationFunctionName("sampleclk"); // model.sim='sampleclk'
	
	setInterfaceFunctionName("SampleCLK");
    
	setDependsOnU(false); // model.dep_ut=[%f %f]
	setDependsOnT(false);
	
	getRealParameters().add(1.0); // model.rpar=[1,0]
	getRealParameters().add(0.0);
	
	setBlockType("d"); // model.blocktype='d'
	
	getExprs().add("1"); // graphics.exprs = [sci2exp(1);sci2exp(0)]
	getExprs().add("0");
    }

}
