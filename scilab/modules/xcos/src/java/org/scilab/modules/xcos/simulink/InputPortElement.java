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

import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkInPort;
import edu.tum.cs.simulink.model.SimulinkLine;

public class InputPortElement {

	private static final Log LOG = LogFactory.getLog(InputPortElement.class);
	
	public InputPortElement(SimulinkBlock base) {
		// TODO Auto-generated constructor stub
	}

	public BasicPort decode(SimulinkInPort simulinkInPort, Object object) {
		// TODO Auto-generated method stub
		if (LOG.isTraceEnabled()) {
			LOG.trace("InPort" + simulinkInPort.toString());
		}
		/*
		 * Set in line
		 */
		
		LineElement lineElement = new LineElement();
		lineElement.decode(simulinkInPort.getLine());
		
		return null;
	}
}
