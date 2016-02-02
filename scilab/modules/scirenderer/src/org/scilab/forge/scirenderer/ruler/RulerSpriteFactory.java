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

package org.scilab.forge.scirenderer.ruler;

import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureManager;

import java.text.DecimalFormat;

/**
 * @author Pierre Lando
 */
public interface RulerSpriteFactory {

    /**
     * Return the texture for the given value.
     * @param value the value.
     * @param adaptedFormat an adapted number format for the given value.
     * @param textureManager {@link TextureManager} to use.
     * @return a positioned text entity for the given value.
     */
    Texture create(double value, DecimalFormat adaptedFormat, TextureManager textureManager);
}
