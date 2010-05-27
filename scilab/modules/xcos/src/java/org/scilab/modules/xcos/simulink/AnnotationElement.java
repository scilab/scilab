package org.scilab.modules.xcos.simulink;

import java.io.IOException;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.TextBlock;

import edu.tum.cs.simulink.model.SimulinkAnnotation;
import edu.tum.cs.simulink.model.SimulinkBlock;

public class AnnotationElement extends AbstractElement<TextBlock>{

	public boolean canDecode(SimulinkAnnotation data) {
		// TODO Auto-generated method stub
		return true;
	}

	public Object decode(SimulinkAnnotation from, Object object) {
		// TODO Auto-generated method stub
		try {
			Logger.toFile("From " + from.getName() + ":");
			Logger.toFile(from.getParameterNames().toString());
		} catch(IOException e1) {
			LogFactory.getLog(BlockElement.class).error(e1);
		}
		return null;
	}

}
