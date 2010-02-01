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

package org.scilab.modules.xcos;

import java.rmi.server.UID;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;

/**
 * @author Bruno JOFRET
 *
 */
public class XcosUIDObject extends mxCell implements Comparable<XcosUIDObject> {

    private static final long serialVersionUID = -2915277403393545917L;

    /**
     * Constructor
     */
    public XcosUIDObject() {
	super();
	setId((new UID()).toString());
    }

    /**
     * 
     */
    public void setId() {
	setId(new UID().toString());
    }

    public void setId(String UID) {
	super.setId(UID);
    }

    public int compareTo(XcosUIDObject o) {
	mxGeometry source = getGeometry();
	mxGeometry target = o.getGeometry();
	
	return (int) ((source.getX() - target.getX()) + (Integer.MAX_VALUE / 2) * (source.getY() - target.getY()));
    }
}
