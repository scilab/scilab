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

package org.scilab.modules.xcos.link.explicit;

import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;

/**
 * An explicit link connect an {@link ExplicitOutputPort} to an
 * {@link ExplicitInputPort}.
 */
public class ExplicitLink extends BasicLink {
    
    private static final long serialVersionUID = 5094752091413277033L;
	// 1 means black color
	// 1 means regular link
	private static final double[][] COLOR_AND_TYPE = {{1.0, 1.0}};

	/** Default constructor */
    public ExplicitLink() {
	super("ExplicitLink");
    }
    
    /** @return A black colored explicit link identifier */
	@Override
    public double[][] getColorAndType() {
	return COLOR_AND_TYPE;
    }
}
