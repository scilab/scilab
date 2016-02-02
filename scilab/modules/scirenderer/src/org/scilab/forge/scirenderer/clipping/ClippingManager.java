/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
