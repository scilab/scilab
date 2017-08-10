/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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

package org.scilab.forge.scirenderer.texture;

import org.scilab.forge.scirenderer.data.DataUser;

/**
 * @author Pierre Lando
 */
public interface Texture extends DataUser {

    /**
     * Enumeration of wrapping possibility.
     */
    public enum Wrap {
        /**
         * REPEAT: the integer part of the texture coordinate to be ignored.
         */
        REPEAT,

        /**
         * CLAMP: the texture coordinate is clamped to [0, 1].
         */
        CLAMP
    }

    /**
     * Enumeration of filtering possibility.
     */
    public enum Filter {
        /**
         * Use the value of the texture element that is nearest (in Manhattan distance) to the center of the pixel being textured.
         */
        NEAREST,

        /**
         * Use the weighted average of the four texture elements that are closest to the center of the pixel being textured.
         */
        LINEAR
    }

    /**
     * Texture validity getter.
     * @return true is this texture is valid and can be used for rendering.
     */
    boolean isValid();

    /**
     * Wrapping mode on the first texture coordinate getter.
     * For more information on wrapping mode {see Wrap}.
     * @return the wrapping mode on the first texture coordinate.
     */
    Wrap getSWrappingMode();

    /**
     * Wrapping mode on the first texture coordinate.
     * @param wrappingMode the new wrapping mode on the first texture coordinate.
     */
    void setSWrappingMode(Wrap wrappingMode);

    /**
     * Wrapping mode on the second dimension getter.
     * If the texture dimension is 1, this method will return <code>null</code>.
     * For more information on wrapping mode {see Wrap}.
     * @return the wrapping mode on the second dimension.
     */
    Wrap getTWrappingMode();

    /**
     * Wrapping mode on the second texture coordinate.
     * @param wrappingMode the new wrapping mode on the second texture coordinate.
     */
    void setTWrappingMode(Wrap wrappingMode);

    /**
     * Minifying filter getter.
     * @return the used minifying filter.
     */
    Filter getMinifyingFilter();

    /**
     * Minifying filter mode setter.
     * @param minifyingFilter the new minifying filter mode.
     */
    void setMinifyingFilter(Filter minifyingFilter);

    /**
     * Magnification filter getter.
     * @return the used magnification filter.
     */
    Filter getMagnificationFilter();

    /**
     * Magnification filter mode setter.
     * @param magnificationFilter the new minifying filter mode.
     */
    void setMagnificationFilter(Filter magnificationFilter);

    /**
     * Texture data provider getter.
     * @return the texture data provider.
     */
    TextureDataProvider getDataProvider();

    /**
     * Texture data provider setter.
     * The texture is set to 'no up to date'.
     * @param provider the new texture data provider.
     */
    void setDataProvider(TextureDataProvider provider);

    /**
     * Set the texture data provider as a drawn texture data provider.
     * @param textureDrawer the given texture drawer.
     */
    void setDrawer(TextureDrawer textureDrawer);

    /**
     * 2D-Texture coordinates must be modified according to the real texture dimension which can differ from
     * the textureSize (with certains GC, a texture must have a size which is a power-of-two).
     * @return the scale factor for the s-coordinate
     */
    double getSScaleFactor();

    /**
     * 2D-Texture coordinates must be modified according to the real texture dimension which can differ from
     * the textureSize (with certains GC, a texture must have a size which is a power-of-two).
     * @return the scale factor for the t-coordinate
     */
    double getTScaleFactor();
}
