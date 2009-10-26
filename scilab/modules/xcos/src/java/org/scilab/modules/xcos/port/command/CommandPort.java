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

package org.scilab.modules.xcos.port.command;

import org.scilab.modules.xcos.port.BasicPort;

public class CommandPort extends BasicPort {
    
    private double initialState = -1.0;
    
    public CommandPort() {
	super("CommandPort");
    }

    public Type getType() {
	return null;
    }

    public void setInitialState(double initialState) {
	this.initialState = initialState;
    }

    public double getInitialState() {
	return initialState;
    }

	public void updateStyle(int angle){
		
		switch(angle){
		case 0 :
			setStyle("CommandPort90");
			break;
		case 90 :
			setStyle("CommandPort180");
			break;
		case 180 :
			setStyle("CommandPort270");
			break;
		case 270 :
			setStyle("CommandPort");
			break;
		}
	}
    
}
