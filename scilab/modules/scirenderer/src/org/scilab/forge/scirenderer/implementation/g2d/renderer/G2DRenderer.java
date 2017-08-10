/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixre DENIZET
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

package org.scilab.forge.scirenderer.implementation.g2d.renderer;

import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.renderer.Renderer;

/**
 * @author Calixte DENIZET
 */
public class G2DRenderer implements Renderer {

    /**
     * The current drawer.
     */
    private Drawer drawer;

    /**
     * Default constructor.
     * The constructor is package : only {@link G2DRendererManager} can instantiate this object.
     */
    G2DRenderer() { }

    @Override
    public void setDrawer(Drawer drawer) {
        this.drawer = drawer;
    }

    @Override
    public Drawer getDrawer() {
        return drawer;
    }

    @Override
    public void reload() {
        // todo : on met qque chose ici ou pas ?
    }

    /**
     * Perform a draw to the given canvas.
     * @param drawingTools the given drawing tools.
     */
    public void draw(DrawingTools drawingTools) {
        if (drawer != null) {
            drawer.draw(drawingTools);
        }
    }
}
