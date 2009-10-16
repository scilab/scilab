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

package org.scilab.modules.xcos.link.commandcontrol;

import org.scilab.modules.xcos.link.BasicLink;


public class CommandControlLink extends BasicLink {
    
    public CommandControlLink() {
	super("CommandControlLink");
    }

    public double[][] getColorAndType() {
	// 5 means red color
	// -1 means activation link
	double[][] result = {{5.0, -1.0}};
	return result;
    }
    
}
