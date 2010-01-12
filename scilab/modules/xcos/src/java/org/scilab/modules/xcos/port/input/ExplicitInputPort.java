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

/**
 * An explicit input port is a port where ... 
 * TODO: document with Serge answer. 
 */
public class ExplicitInputPort extends InputPort {

    private static final long serialVersionUID = 3389508566614965375L;

    /** Default constructor */
    public ExplicitInputPort() {
	super("ExplicitInputPort");
    }

    /** @return always Type.EXPLICIT */
    public Type getType() {
	return Type.EXPLICIT;
    }
    
}
