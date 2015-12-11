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
