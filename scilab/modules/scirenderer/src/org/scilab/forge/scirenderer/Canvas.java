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

package org.scilab.forge.scirenderer;

import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.picking.PickingManager;
import org.scilab.forge.scirenderer.renderer.RendererManager;
import org.scilab.forge.scirenderer.texture.TextureManager;

import java.awt.Dimension;

/**
 * @author Pierre Lando
 */
public interface Canvas {

    /**
     * Set this canvas main drawer.
     * @param mainDrawer the new canvas main drawer.
     */
    void setMainDrawer(Drawer mainDrawer);

    /**
     * Return the canvas main drawer.
     * @return the canvas main drawer.
     */
    Drawer getMainDrawer();

    /**
     * Return the renderer manager.
     * @return the renderer manager.
     */
    RendererManager getRendererManager();

    /**
     * Return the buffers manager of this canvas.
     * @return the buffers manager of this canvas.
     */
    BuffersManager getBuffersManager();

    /**
     * Return the picking manager.
     * @return the picking manager.
     */
    PickingManager getPickingManager();

    /**
     * Texture manger getter.
     * @return the texture manager.
     */
    TextureManager getTextureManager();

    /**
     * Return the canvas width.
     * @return the canvas width.
     */
    int getWidth();

    /**
     * Return the canvas height.
     * @return the canvas height.
     */
    int getHeight();

    /**
     * Return the canvas dimension.
     * @return the canvas dimension.
     */
    Dimension getDimension();

    /**
     * Anti-aliasing level getter.
     *  - 0 for 1x
     *  - 1 for 2x
     *  - 2 for 4x
     *  - 3 for 8x
     *  - 4 for 16x
     * @return the anti-aliasing level.
     */
    public int getAntiAliasingLevel();

    /**
     * Anti-aliasing level setter.
     *  - 0 for 1x
     *  - 1 for 2x
     *  - 2 for 4x
     *  - 3 for 8x
     *  - 4 for 16x
     * @param antiAliasingLevel the new level.
     */
    public void setAntiAliasingLevel(int antiAliasingLevel);

    /** Ask the canvas to perform asynchronous drawing. */
    void redraw();

    /** Ask the canvas to perform asynchronous drawing. */
    void redrawAndWait();

    /** Wait until a drawing has been performed */
    void waitImage();

    /** Destroy canvas : release Semaphore and associated threads */
    void destroy();
}
