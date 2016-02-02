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

import javax.media.opengl.GL2;

/**
 * This class store current OpenGl context capacity.
 *
 * @author Pierre Lando
 */
public class JoGLCapacity {

    /** The actual aliased point size range. */
    private final float[] aliasedPointSizeRange = new float[] {0, 0};

    /** The actual light number. */
    private final  int[] lightNumber = new int[] {8};

    /** The actual light number. */
    private final int[] clippingPlaneNumber = new int[] {6};

    /** Maximum texture size. */
    private final int[] textureSize = new int[] {64};

    /** Accumulation buffer presence */
    private boolean accumulationBufferPresent = false;

    /** ABGR extension */
    private boolean hasABGRExtension = false;

    /**
     * Default constructor.
     * The constructor is package because, only {@link JoGLDrawingTools} use this class.
     */
    JoGLCapacity() {
    }

    /**
     * Reset the OpenGl capacity from the given context.
     * @param gl the given OpenGl context.
     */
    void glReload(GL2 gl) {
        gl.glGetFloatv(GL2.GL_ALIASED_POINT_SIZE_RANGE, aliasedPointSizeRange, 0);
        gl.glGetIntegerv(GL2.GL_MAX_CLIP_PLANES, clippingPlaneNumber, 0);
        gl.glGetIntegerv(GL2.GL_MAX_LIGHTS, lightNumber, 0);
        gl.glGetIntegerv(GL2.GL_MAX_TEXTURE_SIZE, textureSize, 0);

        int[] nbAccumulationBits = {0, 0, 0};
        gl.glGetIntegerv(GL2.GL_ACCUM_RED_BITS, nbAccumulationBits, 0);
        gl.glGetIntegerv(GL2.GL_ACCUM_GREEN_BITS, nbAccumulationBits, 1);
        gl.glGetIntegerv(GL2.GL_ACCUM_BLUE_BITS, nbAccumulationBits, 2);
        if (nbAccumulationBits[0] == 0 || nbAccumulationBits[1] == 0 || nbAccumulationBits[2] == 0) {
            // accumulation buffers not there
            accumulationBufferPresent = false;
        } else {
            accumulationBufferPresent = true;
        }

        hasABGRExtension = gl.isExtensionAvailable("GL_EXT_abgr");
    }

    /**
     * Return the aliased point size range.
     * The returned array had two elements.
     * @return the aliased point size range.
     */
    public float[] getAliasedPointSizeRange() {
        return aliasedPointSizeRange.clone();
    }

    /**
     * Return the number of available light.
     * @return the number of available light.
     */
    public int getLightNumber() {
        return lightNumber[0];
    }

    public int getClippingPlaneNumber() {
        return clippingPlaneNumber[0];
    }

    public int getMaximumTextureSize() {
        return textureSize[0];
    }

    public boolean isAccumulationBufferPresent() {
        return accumulationBufferPresent;
    }

    public boolean isABRExtensionPresent() {
        return hasABGRExtension;
    }
}
