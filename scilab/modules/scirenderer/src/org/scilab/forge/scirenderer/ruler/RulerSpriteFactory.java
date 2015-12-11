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
