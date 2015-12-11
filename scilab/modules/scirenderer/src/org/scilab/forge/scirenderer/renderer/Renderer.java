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

package org.scilab.forge.scirenderer.renderer;

import org.scilab.forge.scirenderer.Drawer;

/**
 *
 * Interface for a renderer.
 *
 * A renderer is used to improve drawing performance.
 * If you have an uge static object to draw at each frame, encapsulate his drawer in a renderer improve drawing performance.
 *
 * @author Pierre Lando
 */
public interface Renderer {

    /**
     * Set the drawer.
     * @param drawer the new drawer.
     */
    void setDrawer(Drawer drawer);

    /**
     * Return the drawer.
     * @return the drawer.
     */
    Drawer getDrawer();

    /**
     * Ask for reload.
     */
    void reload();
}
