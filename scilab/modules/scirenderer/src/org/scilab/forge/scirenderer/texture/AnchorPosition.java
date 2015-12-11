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

package org.scilab.forge.scirenderer.texture;

/**
 * This is an enumeration of possible sprite anchor position.
 * @author Pierre Lando
 */
public enum AnchorPosition {
    /**
     * Anchor is in the upper left sprite corner.
     */
    UPPER_LEFT,

    /**
     * Anchor is in the upper right sprite corner.
     */
    UPPER_RIGHT,

    /**
     * Anchor is in the lower left sprite corner.
     */
    LOWER_LEFT,

    /**
     * Anchor is in the lower right sprite corner.
     */
    LOWER_RIGHT,

    /**
     * Anchor is in the sprite center.
     */
    CENTER,

    /**
     * Anchor is in the center of the right edge of the sprite.
     */
    RIGHT,

    /**
     * Anchor is in the center of the left edge of the sprite.
     */
    LEFT,

    /**
     * Anchor is in the center of the down edge of the sprite.
     */
    DOWN,

    /**
     * Anchor is in the center of the upper edge of the sprite.
     */
    UP,
}
