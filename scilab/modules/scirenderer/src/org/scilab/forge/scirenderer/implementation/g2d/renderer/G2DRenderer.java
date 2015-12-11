/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixre DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
