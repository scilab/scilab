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

package org.scilab.modules.xcos.port.output;

import org.scilab.modules.xcos.port.BasicPort;

public abstract class OutputPort extends BasicPort {

    public OutputPort(String type) {
	super(type);
    }
    
	public void updateStyle(int angle){
		
		switch(angle){
		case 0 :
			super.setStyle("ExplicitOutputPort");
			break;
		case 90 :
			super.setStyle("ExplicitOutputPort90");
			break;
		case 180 :
			super.setStyle("ExplicitOutputPort180");
			break;
		case 270 :
			super.setStyle("ExplicitOutputPort270");
			break;
		}
	}    
}
