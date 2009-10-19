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

package org.scilab.modules.xcos.link.implicit;

import org.scilab.modules.xcos.link.BasicLink;


public class ImplicitLink extends BasicLink {
    
    public ImplicitLink() {
	super("ImplicitLink");
    }
    
    public double[][] getColorAndType() {
	// 1 means red color
	// 2 means implicit link
	double[][] result = {{1.0, 2.0}};
	return result;
    }
}
