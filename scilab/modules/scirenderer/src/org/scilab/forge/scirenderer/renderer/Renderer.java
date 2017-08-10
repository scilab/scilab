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
