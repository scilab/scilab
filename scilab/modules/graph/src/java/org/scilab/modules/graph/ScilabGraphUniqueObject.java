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

package org.scilab.modules.graph;

import java.rmi.server.UID;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;

/**
 * Implement a unique object.
 * 
 * All cells in a ScilabGraph must follow this signature in order to be unique
 * in the graph.
 */
public abstract class ScilabGraphUniqueObject extends mxCell implements Comparable<ScilabGraphUniqueObject> {

    private static final long serialVersionUID = -2915277403393545917L;

    /**
     * Constructor
     */
    public ScilabGraphUniqueObject() {
	super();
	setId((new UID()).toString());
    }

    /**
     * Generate a new UID for this cell
     */
    public void generateId() {
	setId(new UID().toString());
    }

    /**
     * Compare this object with another one.
     * 
     * @param o the object to compare to.
     * @return True if instance are the same; False otherwise.
     * @see java.lang.Comparable#compareTo(java.lang.Object)
     */
    public int compareTo(ScilabGraphUniqueObject o) {
	mxGeometry source = getGeometry();
	mxGeometry target = o.getGeometry();
	
	return (int) ((source.getX() - target.getX()) + (Integer.MAX_VALUE / 2) * (source.getY() - target.getY()));
    }
}
