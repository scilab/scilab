/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte Denizet
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

package org.scilab.forge.scirenderer.implementation.g2d.motor;

import java.util.List;

import org.scilab.forge.scirenderer.tranformations.Vector4d;

/**
 * @author Calixte DENIZET
 *
 * Interface to represent a clippable object.
 */
public interface Clippable {

    /**
     * Break this ConvexObject against a plane
     * @param v plane definition
     * @return a list of ConvexObject.
     */
    public List<ConvexObject> breakObject(Vector4d v);

}