/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.port.input;

import java.util.List;

import org.scilab.modules.xcos.port.BasicPort;

public abstract class InputPort extends BasicPort {

	protected InputPort(String type) {
		super(type);
	}

	public void updateStyle(int angle){
		
		switch(angle){
		case 0 :
			setStyle("ExplicitInputPort180");
			break;
		case 90 :
			setStyle("ExplicitInputPort270");
			break;
		case 180 :
			setStyle("ExplicitInputPort");
			break;
		case 270 :
			setStyle("ExplicitInputPort90");
			break;
		}
	}
}
