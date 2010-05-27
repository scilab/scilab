package org.scilab.modules.xcos.simulink;

import java.io.IOException;
import java.util.Iterator;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.port.BasicPort;

import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkInPort;
import edu.tum.cs.simulink.model.SimulinkLine;

public class InputPortElement {

	public InputPortElement(SimulinkBlock base) {
		// TODO Auto-generated constructor stub
	}

	public BasicPort decode(SimulinkInPort simulinkInPort, Object object) {
		// TODO Auto-generated method stub
		try {
			Logger.toFile("InPort: " + simulinkInPort.toString());
			//Logger.toFile(simulinkOutPort.getParameterNames().toString());
		} catch(IOException e1) {
			LogFactory.getLog(BlockElement.class).error(e1);
		}
		
		/*
		 * Set in line
		 */
		
		LineElement lineElement = new LineElement();
		lineElement.decode(simulinkInPort.getLine());
		
		return null;
	}
}
