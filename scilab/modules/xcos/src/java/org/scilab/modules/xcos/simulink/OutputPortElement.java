package org.scilab.modules.xcos.simulink;

import java.io.IOException;
import java.util.Iterator;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.port.BasicPort;

import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkLine;
import edu.tum.cs.simulink.model.SimulinkOutPort;

public class OutputPortElement {

	public OutputPortElement(SimulinkBlock base) {
		// TODO Auto-generated constructor stub
	}

	public BasicPort decode(SimulinkOutPort simulinkOutPort, Object object) {
		// TODO Auto-generated method stub
		try {
			Logger.toFile("OutPort: " + simulinkOutPort.toString());
			//Logger.toFile(simulinkOutPort.getParameterNames().toString());
		} catch(IOException e1) {
			LogFactory.getLog(BlockElement.class).error(e1);
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
