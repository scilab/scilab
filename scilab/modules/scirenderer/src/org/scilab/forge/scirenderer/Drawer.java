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

package org.scilab.forge.scirenderer;

/**
 * @author Pierre Lando
 */
public interface Drawer {

    /**
     * Ask this drawer to draw with the given drawing tools.
     * @param drawingTools the givens drawing tools.
     */
    void draw(DrawingTools drawingTools);

    /**
     * @return true if it is a 2D drawing
     */
    boolean is2DView();
}
