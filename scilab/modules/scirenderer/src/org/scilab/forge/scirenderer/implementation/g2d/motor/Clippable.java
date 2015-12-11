/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte Denizet
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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