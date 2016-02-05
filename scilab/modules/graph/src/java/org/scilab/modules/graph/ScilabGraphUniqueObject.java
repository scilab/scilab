/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.graph;

import java.awt.geom.Point2D;
import java.io.Serializable;
import java.rmi.server.UID;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;

/**
 * Implement a unique object.
 *
 * All cells in a ScilabGraph must follow this signature in order to be unique
 * in the graph.
 */
public abstract class ScilabGraphUniqueObject extends mxCell implements Comparable<ScilabGraphUniqueObject>, Serializable {

    private static final long serialVersionUID = -2915277403393545917L;

    /**
     * Constructor
     */
    public ScilabGraphUniqueObject() {
        super();
        generateId();
    }

    /**
     * Generated a new id and set it to the current cell.
     *
     * @see com.mxgraph.model.mxCell#setId(java.lang.String)
     */
    public void generateId() {
        super.setId(new UID().toString());
    }

    /**
     * Compare this object with another one.
     *
     * @param o
     *            the object to compare to.
     * @return True if instance are the same; False otherwise.
     * @see java.lang.Comparable#compareTo(java.lang.Object)
     */
    @Override
    public int compareTo(ScilabGraphUniqueObject o) {
        mxGeometry geom1 = getGeometry();
        mxGeometry geom2 = o.getGeometry();

        final Point2D.Double p1 = new Point2D.Double(geom1.getCenterX(), geom1.getCenterY());
        final Point2D.Double p2 = new Point2D.Double(geom2.getCenterX(), geom2.getCenterY());

        return (int) p1.distanceSq(p2);
    }

    /**
     * @return a clone of the cell.
     * @throws CloneNotSupportedException
     *             Thrown to indicate that the clone method in class Object has
     *             been called to clone an object, but that the object's class
     *             does not implement the Cloneable interface.
     * @see com.mxgraph.model.mxCell#clone()
     */
    @Override
    public Object clone() throws CloneNotSupportedException {
        ScilabGraphUniqueObject clone = (ScilabGraphUniqueObject) super.clone();

        /* regenerate a new id for the clone */
        clone.generateId();

        return clone;
    }
}
