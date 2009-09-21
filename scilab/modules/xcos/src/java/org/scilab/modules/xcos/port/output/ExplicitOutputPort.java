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

import com.mxgraph.model.mxGeometry;

public class ExplicitOutputPort extends OutputPort {

    public ExplicitOutputPort() {
	super("ExplicitOutputPort");
	setGeometry(new mxGeometry(80, 35, 10, 10));
    }

    public Type getType() {
	return Type.EXPLICIT;
    }
    
}
