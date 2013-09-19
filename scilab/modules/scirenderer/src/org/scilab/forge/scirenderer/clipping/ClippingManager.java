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

/**
 * Clipping manager interface.
 *
 * @author Pierre Lando
 */
public interface ClippingManager {

    /**
     * Return the number of available clipping plane.
     * At least 6 clipping plane are supported.
     * @return the number of available clipping plane.
     */
    int getClippingPlaneNumber();

    /**
     * Return the i-th clipping plane.
     * {@code null} is returned if i is not a valid index.
     * @param i the given index.
     * @return  the i-th clipping plane.
     */
    ClippingPlane getClippingPlane(int i);

    /**
     * Disable all clipping plane.
     */
    void disableClipping();

    // TODO add an AABB quick call. (with transformation ?)
}
