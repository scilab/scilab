package org.scilab.modules.renderer.JoGLView.postRendering;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;

/**
 * @author Pierre Lando
 */
public interface PostRendered {
    void draw(DrawingTools drawingTools) throws SciRendererException;
}
