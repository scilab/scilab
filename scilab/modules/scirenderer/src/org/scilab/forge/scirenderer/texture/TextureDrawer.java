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

import java.awt.Dimension;

/**
 * @author Pierre Lando
 */
public interface TextureDrawer {

    /**
     * Origin position.
     */
    public enum OriginPosition {
        /**
         * Origin is the sprite center.
         */
        CENTER,

        /**
         * Origin is the upper left sprite corner.
         */
        UPPER_LEFT
    }

    /**
     * Call the texture drawing.
     * @param textureDrawingTools the used drawing tools.
     */
    void draw(TextureDrawingTools textureDrawingTools);

    /**
     * Texture size getter.
     * @return texture size.
     */
    Dimension getTextureSize();

    /**
     * Return the origin position.
     * Warning: change the returned value during a draw call have no effect.
     * @return the origin position.
     */
    OriginPosition getOriginPosition();
}
