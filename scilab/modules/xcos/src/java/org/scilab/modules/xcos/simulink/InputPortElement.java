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

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.port.BasicPort.DataType;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;

import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkInPort;

public class InputPortElement {

	private static final Log LOG = LogFactory.getLog(InputPortElement.class);
	
	public InputPortElement(SimulinkBlock base) {
		// TODO Auto-generated constructor stub
	}

	public InputPort decode(SimulinkInPort simulinkInPort, Object object) {
		
		InputPort port;
		port = allocatePort(simulinkInPort);
		fillParameters(port);
		
		return port;
	}
 
	private void fillParameters(InputPort port) {
		// TODO Get port parameters from simulink
		int nbLines = 1;
		int nbColumns = 1;
		int type = 1;
		
		port.setDataLines(nbLines);
		port.setDataColumns(nbColumns);
		port.setDataType(DataType.convertScilabValue(type));
	}

	private InputPort allocatePort(SimulinkInPort simulinkInPort) { 
		// TODO Auto-generated method stub
		InputPort ret;
		ret = new ExplicitInputPort();
		ret.setId("Input"+simulinkInPort.toString());
		if (LOG.isTraceEnabled()) {
			LOG.trace("InPort" + ret);	
			LOG.trace("InPort" + ret.getId());
		}
		return ret;
	}
}
