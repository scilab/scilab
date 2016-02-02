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

package org.scilab.forge.scirenderer.implementation.jogl;

import org.scilab.forge.scirenderer.Canvas;

import javax.media.opengl.GLAutoDrawable;

/**
 * @author Pierre Lando
 */
public final class JoGLCanvasFactory {

    /**
     * Private constructor.
     * This is an utility class.
     */
    private JoGLCanvasFactory() {

    }

    /**
     * Create a canvas from an auto drawable object.
     * @param autoDrawable the auto drawable object.
     * @return a canvas based on the given auto drawable object.
     */
    public static Canvas createCanvas(final GLAutoDrawable autoDrawable) {
        return new JoGLCanvas(autoDrawable);
    }

    /**
     * Create a canvas for an offscreen rendering.
     * @param width the width
     * @param height the height
     * @return a canvas.
     */
    public static Canvas createCanvas(final int width, final int height) {
        return new JoGLCanvas(width, height);
    }
}
