package org.scilab.modules.xcos.simulink;

import java.io.IOException;
import java.util.Iterator;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.port.BasicPort;

import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkLine;
import edu.tum.cs.simulink.model.SimulinkOutPort;

public class OutputPortElement {

	private static final Log LOG = LogFactory.getLog(OutputPortElement.class);
	
	public OutputPortElement(SimulinkBlock base) {
		// TODO Auto-generated constructor stub
	}

	public BasicPort decode(SimulinkOutPort simulinkOutPort, Object object) {
		// TODO Auto-generated method stub
		if (LOG.isTraceEnabled()) {
			LOG.trace("InPort" + simulinkOutPort.toString());
		}
		/*
		 * Set out lines
		 */
		LineElement lineElement = new LineElement();
		Iterator<SimulinkLine> lineOutIter = simulinkOutPort.getLines().iterator();
	    while(lineOutIter.hasNext()) {
    		lineElement.decode(lineOutIter.next());
	    }
		
		return null;
	}

}
