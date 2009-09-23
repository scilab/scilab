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
package org.scilab.modules.xcos.block.scope;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;

public class ScopeBlock extends BasicBlock {

    public ScopeBlock() {
	super("Scope", "Scope");
	InputPort inputPort = new ExplicitInputPort();
	ControlPort controlPort = new ControlPort();
	
	inputPort.setDataLines(-1); // model.in
	inputPort.setDataColumns(1); // model.in2
	addPort(inputPort);
	
	controlPort.setDataLines(1); // model.evtin
	addPort(controlPort);
	
	setInterfaceFunctionName("CSCOPE");
	
	setSimulationFunctionName("cscope");
	setSimulationFunctionType(SimulationFunctionType.C_OR_FORTRAN);
	
	getRealParameters().add(0.0); // model.rpar
	getRealParameters().add(-15.0);
	getRealParameters().add(15.0);
	getRealParameters().add(30.0);
	
	getIntegerParameters().add(-1); // model.ipar
	getIntegerParameters().add(1);
	getIntegerParameters().add(20);
	getIntegerParameters().add(1);
	getIntegerParameters().add(3);
	getIntegerParameters().add(5);
	getIntegerParameters().add(7);
	getIntegerParameters().add(9);
	getIntegerParameters().add(11);
	getIntegerParameters().add(13);
	getIntegerParameters().add(15);
	getIntegerParameters().add(-1);
	getIntegerParameters().add(-1);
	getIntegerParameters().add(600);
	getIntegerParameters().add(400);
	
	setDependsOnU(true); // model.dep_ut=[%t %f]
	setDependsOnT(false);
    }
}
