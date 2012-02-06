/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.axes.ruler;

import org.scilab.forge.scirenderer.ruler.RulerSpriteFactory;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteDrawer;
import org.scilab.forge.scirenderer.sprite.SpriteDrawingTools;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.forge.scirenderer.sprite.TextEntity;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.utils.textRendering.FontManager;

import java.awt.Dimension;
import java.awt.Font;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.text.NumberFormat;

/**
 * This implementation of {@see RulerSpriteFactory} create ruler labels for the given {@see Axes}.
 *
 * @author Pierre Lando
 */
class AxesRulerSpriteFactory implements RulerSpriteFactory {

    /**
     * This factory create ruler label for this {@see Axes}.
     */
    private final Axes axes;

    /**
     * The current colormap.
     */
    private final ColorMap colorMap;

    /**
     * Default constructor.
     * @param axes This factory create ruler label for this given {@see Axes}.
     */
    public AxesRulerSpriteFactory(Axes axes) {
        this.axes = axes;
        ColorMap figureColorMap;
        try {
            GraphicController controller = GraphicController.getController();
            Figure parentFigure = (Figure) controller.getObjectFromId(axes.getParentFigure());
            figureColorMap = parentFigure.getColorMap();
        } catch (NullPointerException e) {
            figureColorMap = null;
        }
        this.colorMap = figureColorMap;
    }

    @Override
    public Sprite create(double value, NumberFormat adaptedFormat, SpriteManager spriteManager) {

        DecimalFormatSymbols dfs = new DecimalFormatSymbols();
        dfs.setDecimalSeparator('.');
        ((DecimalFormat) adaptedFormat).setDecimalFormatSymbols(dfs);

        String text = adaptedFormat.format(value).replaceAll("E", "e");
        final TextEntity textEntity = new TextEntity(text);

        textEntity.setTextAntiAliased(false);
        textEntity.setTextUseFractionalMetrics(axes.getFontFractional());
        textEntity.setTextColor(ColorFactory.createColor(colorMap, axes.getFontColor()));

        Font font = FontManager.getSciFontManager().getFontFromIndex(axes.getFontStyle(), axes.getFontSize());
        textEntity.setFont(font);

        Dimension dimension = spriteManager.getSize(textEntity);
        Sprite sprite = spriteManager.createSprite(dimension.width + 1, dimension.height + 1);
        sprite.setDrawer(new SpriteDrawer() {

            @Override
            public void draw(SpriteDrawingTools drawingTools) {
                drawingTools.draw(textEntity, 0, 0);
            }

            @Override
            public OriginPosition getOriginPosition() {
                return SpriteDrawer.OriginPosition.UPPER_LEFT;
            }
        });

        return sprite;
    }
}
