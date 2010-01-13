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
 * An implicit input port is a port where ... 
 * TODO: document with Serge answer. 
 */
public class ImplicitInputPort extends InputPort {

    private static final long serialVersionUID = 8711291857895989304L;

    /** Default constructor */
    public ImplicitInputPort() {
	super("ImplicitInputPort");
    }

    /** @return always Type.IMPLICIT */
    public Type getType() {
	return Type.IMPLICIT;
    }
    
}
