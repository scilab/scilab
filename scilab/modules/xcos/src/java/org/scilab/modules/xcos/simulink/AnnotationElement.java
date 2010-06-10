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
		LogFactory.getLog(AnnotationElement.class).trace("From " + from.getName() + ":");
		LogFactory.getLog(AnnotationElement.class).trace(from.getParameterNames().toString());
		return null;
	}

}
