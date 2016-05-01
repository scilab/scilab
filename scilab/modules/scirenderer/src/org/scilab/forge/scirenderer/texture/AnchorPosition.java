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
