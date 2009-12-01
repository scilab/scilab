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

    private static final long serialVersionUID = -8098437925667788997L;

    public OutputPort(String type) {
	super(type);
	setInitialAngle(0);
    }
}
