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

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.link.BasicLink;

import edu.tum.cs.simulink.model.SimulinkLine;

public class LineElement extends AbstractElement<BasicLink>{

	private static final Log LOG = LogFactory.getLog(LineElement.class);
	
	public void decode(SimulinkLine simulinkLine) {
		// TODO Auto-generated method stub
		if (LOG.isTraceEnabled()) {
			LOG.trace("Line: " + simulinkLine.toString());
		}
	}

}
