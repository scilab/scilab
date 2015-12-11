/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.g2d.renderer;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.renderer.Renderer;
import org.scilab.forge.scirenderer.renderer.RendererManager;

/**
 * @author Calixte DENIZET
 */
public class G2DRendererManager implements RendererManager {

    /**
     * Default constructor.
     */
    public G2DRendererManager() {
    }

    @Override
    public Renderer createRenderer() {
        G2DRenderer newRenderer = new G2DRenderer();
        return newRenderer;
    }

    @Override
    public void dispose(Renderer renderer) { }

    /**
     * Perform a draw with the given renderer to the given canvas..
     * @param drawingTools the given drawing tools.
     * @param renderer the given renderer.
     */
    public void draw(DrawingTools drawingTools, Renderer renderer) {
        if (renderer instanceof G2DRenderer) {
            ((G2DRenderer) renderer).draw(drawingTools);
        }
    }
}
