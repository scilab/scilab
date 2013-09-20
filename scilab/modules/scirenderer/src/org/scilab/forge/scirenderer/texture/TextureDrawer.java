/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
