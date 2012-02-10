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
import org.scilab.modules.graphic_objects.axes.AxisProperty;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.textObject.FormattedText;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.JoGLView.util.FormattedTextSpriteDrawer;
import org.scilab.modules.renderer.utils.textRendering.FontManager;

import java.awt.Dimension;
import java.awt.Font;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;

/**
 * This implementation of {@see RulerSpriteFactory} create ruler labels for the given {@see Axes}.
 *
 * @author Pierre Lando
 */
class AxesRulerSpriteFactory implements RulerSpriteFactory {
    /**
     * The symbol used for ticks label in log and auto ticks mode.
     */
    private static final String MULTIPLICATION_SYMBOL = "x";

    /**
     * The exponent size is smaller than the mantissa size.
     */
    private static final float EXPONENT_SIZE_RATIO = 0.4f;

    /**
     * This factory create ruler label for this {@see Axis}.
     */
    private final AxisProperty axisProperty;

    /**
     * The current colormap.
     */
    private final ColorMap colorMap;

    /**
     * Default constructor.
     * @param axes This factory create ruler label for one axis of this given {@see Axes}.
     * @param axisId the id of the managed axis.
     */
    public AxesRulerSpriteFactory(Axes axes, int axisId) {
        this.axisProperty = axes.getAxes()[axisId];
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
    public Sprite create(double value, DecimalFormat adaptedFormat, SpriteManager spriteManager) {
        //System.out.println((axisProperty.getAutoTicks() ? "auto-" : "mano-") + (axisProperty.getLogFlag() ? "log" : "lin") + " @" + value);
        if (axisProperty.getAutoTicks()) {
            setScilabStyle(adaptedFormat);
            if (axisProperty.getLogFlag()) {
                return createScientificStyleSprite(value, spriteManager);
            } else {
                return createSimpleSprite(adaptedFormat.format(value), spriteManager);
            }
        } else {
            FormattedText formattedText = getTextAtValue(value);
            FormattedTextSpriteDrawer textObjectSpriteDrawer = new FormattedTextSpriteDrawer(colorMap, spriteManager, formattedText);
            Sprite sprite = spriteManager.createSprite(textObjectSpriteDrawer.getSpriteSize());
            sprite.setDrawer(textObjectSpriteDrawer);
            return sprite;
        }
    }

    /**
     * Set the given {@see DecimalFormat} to scilab style.
     * @param format the given {@see DecimalFormat}.
     */
    private void setScilabStyle(DecimalFormat format) {
        DecimalFormatSymbols decimalFormatSymbols = format.getDecimalFormatSymbols();
        decimalFormatSymbols.setDecimalSeparator('.');
        decimalFormatSymbols.setExponentSeparator("e");
        format.setDecimalFormatSymbols(decimalFormatSymbols);
    }

    /**
     * Create and return a sprite representing the given value.
     * The returned sprites will look like "5x10²"
     * @param value, the given value.
     * @param spriteManager used sprite manager.
     * @return a simple sprite representing the given value with the adapted format.
     */
    private Sprite createScientificStyleSprite(double value, SpriteManager spriteManager) {
        Integer exponent = (int) Math.floor(Math.log10(value));
        Double mantissa = value / Math.pow(10, exponent);

        /**
         * Create mantissa.
         */
        final TextEntity mantissaTextEntity;
        if (mantissa != 1) {
            mantissaTextEntity = new TextEntity(mantissa.toString() + MULTIPLICATION_SYMBOL + "10");
        } else {
            mantissaTextEntity = new TextEntity("10");
        }

        Font mantissaFont = FontManager.getSciFontManager().getFontFromIndex(axisProperty.getFontStyle(), axisProperty.getFontSize());
        mantissaTextEntity.setTextAntiAliased(false);
        mantissaTextEntity.setTextUseFractionalMetrics(axisProperty.getFontFractional());
        mantissaTextEntity.setTextColor(ColorFactory.createColor(colorMap, axisProperty.getFontColor()));
        mantissaTextEntity.setFont(mantissaFont);
        final Dimension mantissaSize = spriteManager.getSize(mantissaTextEntity);

        /**
         * Create exponent.
         */
        final TextEntity exponentTextEntity = new TextEntity(exponent.toString());
        Font exponentFont = FontManager.getSciFontManager().getFontFromIndex(axisProperty.getFontStyle(), axisProperty.getFontSize() * EXPONENT_SIZE_RATIO);
        exponentTextEntity.setTextAntiAliased(false);
        exponentTextEntity.setTextUseFractionalMetrics(axisProperty.getFontFractional());
        exponentTextEntity.setTextColor(ColorFactory.createColor(colorMap, axisProperty.getFontColor()));
        exponentTextEntity.setFont(exponentFont);
        final Dimension exponentSize = spriteManager.getSize(exponentTextEntity);

        Sprite sprite = spriteManager.createSprite(
                exponentSize.width + mantissaSize.width,
                exponentSize.height + mantissaSize.height
        );

        sprite.setDrawer(new SpriteDrawer() {

            @Override
            public void draw(SpriteDrawingTools drawingTools) {
                drawingTools.draw(mantissaTextEntity, 0, exponentSize.height);
                drawingTools.draw(exponentTextEntity, mantissaSize.width, 0);
            }

            @Override
            public OriginPosition getOriginPosition() {
                return SpriteDrawer.OriginPosition.UPPER_LEFT;
            }
        });

        return sprite;
    }

    /**
     * Create and return a simple sprite representing the given value.
     * @param text the formatted string representing the value.
     * @param spriteManager used sprite manager.
     * @return a simple sprite representing the given value with the adapted format.
     */
    private Sprite createSimpleSprite(String text, SpriteManager spriteManager) {
        Font font = FontManager.getSciFontManager().getFontFromIndex(axisProperty.getFontStyle(), axisProperty.getFontSize());
        final TextEntity textEntity = new TextEntity(text);
        textEntity.setTextAntiAliased(false);
        textEntity.setTextUseFractionalMetrics(axisProperty.getFontFractional());
        textEntity.setTextColor(ColorFactory.createColor(colorMap, axisProperty.getFontColor()));
        textEntity.setFont(font);

        Dimension dimension = spriteManager.getSize(textEntity);
        Sprite sprite = spriteManager.createSprite(dimension.width, dimension.height);
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

    /**
     * Return the user defined {@see FormattedText} ticks label corresponding to the given value.
     * @param value the given value.
     * @return the user defined {@see FormattedText} ticks label corresponding to the given value.
     */
    private FormattedText getTextAtValue(double value) {
        Double[] locations = axisProperty.getTicksLocations();
        int index = -1;
        for (int i = 0 ; i < locations.length ; i++) {
            if (locations[i] == value) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            return null;
        } else {
            return axisProperty.getTicksLabels().get(index);
        }
    }
}
