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

package org.scilab.forge.scirenderer.tranformations;

/**
 * @author Pierre Lando
 */
public interface Transformation {

    /**
     * Return this right times the given transformation.
     * @param transformation the given transformation.
     * @return a new transformation.
     */
    Transformation rightTimes(Transformation transformation);

    /**
     * Return this left times the given transformation.
     * @param transformation the given transformation.
     * @return a new transformation.
     */
    Transformation leftTimes(Transformation transformation);

    /**
     * Return the inverse transformation.
     * @return the inverse transformation.
     */
    Transformation getInverseTransformation();

    /**
     * Project the given vector.
     * W value is set to 1.
     * @param vector the given vector.
     * @return the given vector projected.
     */
    Vector3d project(Vector3d vector);

    /**
     * Project the given direction.
     * Translation part is not used.
     * @param direction the direction to project.
     * @return the given direction projected.
     */
    Vector3d projectDirection(Vector3d direction);

    /**
     * Unproject the given vector.
     * W value is set to 1.
     * @param vector the given vector.
     * @return the given vector un-projected.
     */
    Vector3d unproject(Vector3d vector);

    /**
     * Return true if is identity, false otherwise.
     * @return true if is identity, false otherwise.
     */
    boolean isIdentity();

    /**
     * Return this transformation matrix.
     * The returned array is a clone of the transformation array.
     * @return this transformation matrix.
     */
    double[] getMatrix();

    /**
     * Return this transformation inverse matrix.
     * The returned array is a clone of the transformation array.
     * @return this transformation inverse matrix.
     */
    double[] getInverseMatrix();
}
