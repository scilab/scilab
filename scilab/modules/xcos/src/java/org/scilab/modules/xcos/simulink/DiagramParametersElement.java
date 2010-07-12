package org.scilab.modules.xcos.simulink;

import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;

public class DiagramParametersElement extends org.scilab.modules.xcos.simulink.AbstractElement<ScicosParameters>{

	
	
	
	/**
	 * Decode the element into the instance
	 * 
	 * @param element the element to be decoded
	 * @param into the Xcos associated instance
	 * @return the modified into parameters
	 * @throws ScicosFormatException on decode error
	 * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.scilabTypes.ScilabType, java.lang.Object)
	 */
	public ScicosParameters decode(ScicosParameters into)
			throws ScicosFormatException {
		
		// Validate the fields
		validate();
		
		/*
		 * fill data
		 */
		//fillFields(into);
		into = new ScicosParameters();
		return into;
	}
	private void fillFields(ScicosParameters into) {
		// TODO Auto-generated method stub
		
	}
	private void validate() {
		// TODO Auto-generated method stub
	}
}
