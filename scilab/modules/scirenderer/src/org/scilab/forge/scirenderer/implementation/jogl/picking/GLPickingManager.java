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

package org.scilab.forge.scirenderer.implementation.jogl.picking;

import javax.media.opengl.GL;

/**
 * @author Pierre Lando
 */
public class GLPickingManager {

    /**
     * The last used unique color.
     */
    private UniqueColor uniqueColor;

    /**
     * Default constructor.
     */
    public GLPickingManager() {
    }


    /**
     * This class make an unique color from an index.
     */
    private class UniqueColor {
        private final int redSize;
        private final int greenSize;
        private final int blueSize;
        private final GL gl;

        /**
         * Constructor.
         * @param gl the used OpenGl context.
         */
        public UniqueColor(GL gl) {
            this.gl = gl;
            int[] colorBits = new int[3];
            gl.glGetIntegerv(GL.GL_RED_BITS, colorBits, 0);
            gl.glGetIntegerv(GL.GL_GREEN_BITS, colorBits, 1);
            gl.glGetIntegerv(GL.GL_BLUE_BITS, colorBits, 2);

            redSize = 1 << colorBits[0];
            greenSize = 1 << colorBits[1];
            blueSize = 1 << colorBits[2];
        }

        /**
         * Set the color by index.
         * @param index given index.
         */
        public void setColor(int index) {
            int i = index;
            float b = (i % blueSize) / (blueSize - 1f);
            i = i >> blueSize;
            float g = (i % greenSize) / (greenSize - 1f);
            i = i >> greenSize;
            float r = (i % redSize) / (redSize - 1f);

            //gl.glColor3f(r, g, b);
            gl.glClearColor(r, g, b, 0);
        }

        /**
         * Get index from color.
         * @param r red component of the color.
         * @param g green component of the color.
         * @param b blue component of the color.
         * @return the index corresponding to the given color.
         */
        public int getIndex(float r, float g, float b) {
            return (int) (((r * (redSize - 1f) + g) * (greenSize - 1f) + b) * (blueSize - 1f));
        }
    }
}
