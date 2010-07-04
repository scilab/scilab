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

import java.io.IOException;
import java.util.Iterator;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkLine;
import edu.tum.cs.simulink.model.SimulinkOutPort;

public class OutputPortElement {

	private static final Log LOG = LogFactory.getLog(OutputPortElement.class);
	
	public OutputPortElement(SimulinkBlock base) {
		// TODO Auto-generated constructor stub
	}

	public OutputPort decode(SimulinkOutPort simulinkOutPort, Object object) {
		// TODO Auto-generated method stub
		OutputPort port;
		port = allocatePort(simulinkOutPort);
		/*
		 * Set out lines
		 */
		
		return port;
	}

	private OutputPort allocatePort(SimulinkOutPort simulinkOutPort) {
		// TODO Auto-generated method stub
		OutputPort ret;
		ret = new ExplicitOutputPort();
		ret.setId(simulinkOutPort.toString());
		if (LOG.isTraceEnabled()) {
			LOG.trace("OutPort" + ret);	
			LOG.trace("OutPort" + ret.getId());
		}
		return ret;
	}

}
