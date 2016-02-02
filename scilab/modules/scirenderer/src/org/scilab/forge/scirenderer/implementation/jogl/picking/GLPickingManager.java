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
