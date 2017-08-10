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

package org.scilab.forge.scirenderer.implementation.jogl.renderer;

import org.scilab.forge.scirenderer.implementation.jogl.JoGLDrawingTools;
import org.scilab.forge.scirenderer.renderer.Renderer;
import org.scilab.forge.scirenderer.renderer.RendererManager;

import javax.media.opengl.GL2;
import java.util.HashSet;
import java.util.Set;
import java.util.Stack;

/**
 * @author Pierre Lando
 */
public class JoGLRendererManager implements RendererManager {

    /**
     * Set of current renderer.
     */
    private final Set<JoGLRenderer> rendererSet = new HashSet<JoGLRenderer>();

    /**
     * Set of dead buffers.
     */
    private final Stack<JoGLRenderer> deadRendererSet = new Stack<JoGLRenderer>();

    /**
     * Default constructor.
     */
    public JoGLRendererManager() {
    }

    @Override
    public Renderer createRenderer() {
        JoGLRenderer newRenderer = new JoGLRenderer();
        rendererSet.add(newRenderer);
        return newRenderer;
    }

    @Override
    public void dispose(Renderer renderer) {
        if ((renderer != null) && (renderer instanceof JoGLRenderer)) {
            JoGLRenderer localRenderer = (JoGLRenderer) renderer;
            rendererSet.remove(localRenderer);
            deadRendererSet.push(localRenderer);
        }
    }

    public void dispose(JoGLDrawingTools drawingTools, Renderer renderer) {
        if ((renderer != null) && (renderer instanceof JoGLRenderer)) {
            JoGLRenderer localRenderer = (JoGLRenderer) renderer;
            localRenderer.dispose(drawingTools.getGl().getGL2());
            rendererSet.remove(localRenderer);
            deadRendererSet.push(localRenderer);
        }
    }

    /**
     * Perform a draw with the given renderer to the given canvas..
     * @param drawingTools the given drawing tools.
     * @param renderer the given renderer.
     */
    public void draw(JoGLDrawingTools drawingTools, Renderer renderer) {
        if ((renderer != null) && (renderer instanceof JoGLRenderer)) {
            ((JoGLRenderer) renderer).draw(drawingTools);
        }
    }

    /**
     * Ask all {@link JoGLRenderer} to reload.
     * This is needed when the OpenGl context has been lost.
     */
    public void glReload() {
        for (JoGLRenderer renderer : rendererSet) {
            renderer.reload();
        }
    }

    /**
     * Synchronize to OpenGl context.
     * Mostly consist to dispose dead renderer resources.
     * @param gl the current OpenGl context.
     */
    public void glSynchronize(GL2 gl) {
        while (!deadRendererSet.isEmpty()) {
            deadRendererSet.pop().dispose(gl);
        }
    }
}
