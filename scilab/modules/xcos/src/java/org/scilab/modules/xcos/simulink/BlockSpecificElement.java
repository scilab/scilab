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
import org.scilab.modules.xcos.block.BasicBlock;

import edu.tum.cs.commons.collections.UnmodifiableIterator;
import edu.tum.cs.simulink.model.SimulinkBlock;

public class BlockSpecificElement {
	PatternElement patternElement = new PatternElement();
	private static final Log LOG = LogFactory.getLog(BlockSpecificElement.class);
	
	public static void print(SimulinkBlock from) {
		if (LOG.isTraceEnabled()) {
			LOG.trace("From " + from.getName() + ":");
			UnmodifiableIterator<String> paramNameIter = from.getParameterNames().iterator();
			while(paramNameIter.hasNext()){
				String paramName = paramNameIter.next();
				LOG.trace(paramName + ": " + from.getParameter(paramName));
			}
		}
	}
	
	public BasicBlock decode(SimulinkBlock from ,BasicBlock into) {
		
		/**
		 * mutable field used to share decoded BasicBlock with submethods
		 */
		BasicBlock base = into;
		if (LOG.isTraceEnabled()) {
			LOG.trace("From " + from.getName() + ":");
			UnmodifiableIterator<String> paramNameIter = from.getParameterNames().iterator();
			while(paramNameIter.hasNext()){
				String paramName = paramNameIter.next();
				LOG.trace(paramName + ": " + from.getParameter(paramName) + " || " + patternElement.decode(from.getName(), paramName.toString(), from.getParameter(paramName)));
			}
		}
		
		/*
		 * fill the data
		 */
		//fillSpecificParameters(base);
		//findCorrespondingParameter(string simulinkParameter);
		
		return into;
	}
}