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
