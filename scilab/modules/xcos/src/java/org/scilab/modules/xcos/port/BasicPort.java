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

package org.scilab.modules.xcos.port;

import com.mxgraph.model.mxCell;

public abstract class BasicPort extends mxCell {

    private int dataSize = 0;
    
    public static enum Type { IMPLICIT, EXPLICIT };
    
    public BasicPort(String style) {
	super();
	setVertex(true);
	setStyle(style);
    }

    public int getDataSize() {
        return dataSize;
    }

    public void setDataSize(int dataSize) {
        this.dataSize = dataSize;
    }
    
    public abstract Type getType();
}
