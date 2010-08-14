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
import org.scilab.modules.xcos.block.TextBlock;

import edu.tum.cs.simulink.model.SimulinkAnnotation;
import edu.tum.cs.simulink.model.SimulinkBlock;

public class AnnotationElement extends AbstractElement<TextBlock>{

	private BlockGraphicElement graphicElement = new BlockGraphicElement();
	private static final Log LOG = LogFactory.getLog(AnnotationElement.class);
	
	public boolean canDecode(SimulinkAnnotation data) {
		// TODO Auto-generated method stub
		return true;
	}
	/**
	 * Parent function for TextBlock decoding
	 * @param from
	 * @param object
	 * @return
	 */
	public Object decode(SimulinkAnnotation from, Object object) {
		// TODO Auto-generated method stub
		if(LOG.isTraceEnabled()) {
			LOG.trace("Decoding annotation: " + from.getName() + ".");
		}
		TextBlock annotation;
		
		annotation = new TextBlock();
		annotation.setValue(from.getName());
		graphicElement.decode(from, annotation);
		
		return annotation;
	}

}
