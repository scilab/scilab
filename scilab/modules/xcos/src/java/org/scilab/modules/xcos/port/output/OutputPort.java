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

import com.mxgraph.model.mxGeometry;

public class OutputPort extends BasicPort {

    public OutputPort() {
	super("outputPort");
	setGeometry(new mxGeometry(80, 35, 10, 10));
    }
    
}
