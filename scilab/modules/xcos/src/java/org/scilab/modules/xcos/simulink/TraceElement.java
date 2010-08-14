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
import java.util.ArrayList;
import java.util.Iterator;

import edu.tum.cs.commons.collections.UnmodifiableIterator;
import edu.tum.cs.simulink.model.SimulinkBlock;

public class TraceElement {

	private static final Log LOG = LogFactory.getLog(TraceElement.class);
	
	private ArrayList<String> unknownBlocks = new ArrayList<String>();
	/*
	 * Array of strings containing block name + fault parameters name
	 */
	private ArrayList<String> faultParameters = new ArrayList<String>();
	/**
	 * 
	 * @param unknownBlock
	 */
	public void addUnknownBlock(String unknownBlock) {
		if(!unknownBlocks.contains(unknownBlock)) {;
			unknownBlocks.add(unknownBlock);
		}
	}
	/**
	 * 
	 * @param faultParameter
	 */
	public void addFaultParameters(String faultParameter) {
		faultParameters.add(faultParameter);
	}
	/**
	 * Funcion printing report after diagram importation, data for report is collected from all errors encountered during translation
	 */
	public void printReport() {
		if(LOG.isTraceEnabled()) {
			LOG.trace("Missing compatibility patterns for " + unknownBlocks.size() +" blocks:");
			Iterator<String> unknownBlockIter = unknownBlocks.iterator();
			while(unknownBlockIter.hasNext()) {
				LOG.trace(unknownBlockIter.next());
			}
			LOG.trace("Errors encountered when translating  " + faultParameters.size() +" blocks");
			Iterator<String> faultParamIter = faultParameters.iterator();
			while(faultParamIter.hasNext()) {
				LOG.trace(faultParamIter.next());
			}
		}
	}
	/**
	 * Function printing all the parameters and their values from SimulinkBlock, it makes easier to create misiing compatibility patterns
	 * @param from
	 */
	public static void printBasicBlockParameters(SimulinkBlock from) {
		if (LOG.isTraceEnabled()) {
			LOG.trace("From " + from.getName() + ":");
			UnmodifiableIterator<String> paramNameIter = from.getParameterNames().iterator();
			while(paramNameIter.hasNext()){
				String paramName = paramNameIter.next();
				LOG.trace(paramName + ": " + from.getParameter(paramName));
			}
		}
	}
}

