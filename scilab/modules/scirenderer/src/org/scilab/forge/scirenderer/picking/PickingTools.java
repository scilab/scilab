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

package org.scilab.forge.scirenderer.picking;

import java.awt.Point;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * Interface for picking tools.
 *
 * @author Pierre Lando
 */
public interface PickingTools {

    /**
     * Return the scene coordinate of the pixel under the given windows position.
     * @param windowPosition the given window position.
     * @return the scene coordinate of the pixel under the given windows position.
     */
    Vector3d getUnderlyingPoint(Point windowPosition);

    /**
     * Return the canvas.
     * @return the canvas.
     */
    Canvas getCanvas();
}
