/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
