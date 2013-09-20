/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.clipping;

import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector4d;

/**
 * @author Pierre Lando
 */
public interface ClippingPlane {

    /**
     * Return the status of this clipping plane.
     * @return the status of this clipping plane.
     */
    boolean isEnable();

    /**
     * Set the status of this clipping plane.
     * @param isEnable new status of this clipping plane.
     */
    void setEnable(boolean isEnable);

    /**
     * Set the clipping plane equation.
     * @param v the plane coordinate.
     */
    void setEquation(Vector4d v);

    /**
     * Return the plane equation.
     * @return the plane equation.
     */
    Vector4d getEquation();

    /**
     * Set the coordinate transformation for the plane.
     * @param transformation the new coordinate transformation for the plane.
     */
    void setTransformation(Transformation transformation);

    /**
     * Return the coordinate transformation for the plane.
     * @return the coordinate transformation for the plane.
     */
    Transformation getTransformation();

    /**
     * Return the clipping plane index.
     * @return the clipping plane index.s
     */
    int getIndex();
}
