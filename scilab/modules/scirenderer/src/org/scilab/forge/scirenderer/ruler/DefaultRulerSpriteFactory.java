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

import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.texture.TextEntity;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.forge.scirenderer.texture.TextureManager;

import java.awt.Dimension;
import java.text.DecimalFormat;

/**
 * @author Pierre Lando
 */
public final class DefaultRulerSpriteFactory implements RulerSpriteFactory {

    /**
     * Default constructor.
     */
    public DefaultRulerSpriteFactory() {
    }

    @Override
    public Texture create(double value, DecimalFormat adaptedFormat, TextureManager textureManager) {
        String text = adaptedFormat.format(value);
        final TextEntity textEntity = new TextEntity(text);
        textEntity.setTextColor(new Color(0, 0, 0));
        textEntity.setTextAntiAliased(false);

        Texture texture = textureManager.createTexture();
        texture.setDrawer(new TextureDrawer() {

            @Override
            public void draw(TextureDrawingTools drawingTools) {
                drawingTools.draw(textEntity, 0, 0);
            }

            @Override
            public Dimension getTextureSize() {
                return textEntity.getSize();
            }

            @Override
            public OriginPosition getOriginPosition() {
                return OriginPosition.UPPER_LEFT;
            }
        });

        return texture;
    }
}
