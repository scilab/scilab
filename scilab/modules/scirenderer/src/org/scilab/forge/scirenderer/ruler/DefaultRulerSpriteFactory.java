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
