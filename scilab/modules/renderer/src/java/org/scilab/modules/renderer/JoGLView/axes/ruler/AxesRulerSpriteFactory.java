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
import org.scilab.forge.scirenderer.texture.TextEntity;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.forge.scirenderer.texture.TextureManager;
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
    public Texture create(double value, DecimalFormat adaptedFormat, TextureManager textureManager) {
        // Simple ack to avoid ticks with "-0" as label.
        if (value == -0) {
            value = 0;
        }

        if (axisProperty.getAutoTicks()) {
            setScilabStyle(adaptedFormat);
            if (axisProperty.getLogFlag()) {
                return createScientificStyleSprite(value, textureManager);
            } else {
                return createSimpleSprite(adaptedFormat.format(value), textureManager);
            }
        } else {
            FormattedText formattedText = getTextAtValue(value);
            FormattedTextSpriteDrawer textObjectSpriteDrawer = new FormattedTextSpriteDrawer(colorMap, formattedText);
            Texture texture = textureManager.createTexture();
            texture.setDrawer(textObjectSpriteDrawer);
            return texture;
        }
    }

    /**
     * Set the given {@see DecimalFormat} to scilab style.
     * @param format the given {@see DecimalFormat}.
     */
    public static void setScilabStyle(DecimalFormat format) {
        DecimalFormatSymbols decimalFormatSymbols = format.getDecimalFormatSymbols();
        decimalFormatSymbols.setDecimalSeparator('.');
        decimalFormatSymbols.setExponentSeparator("e");
        format.setDecimalFormatSymbols(decimalFormatSymbols);
    }

    /**
     * Create and return a texture representing the given value.|| index >= axisProperty.getTicksLabels().size()
     * The returned sprites will look like "5x10^2"
     * @param value, the given value.
     * @param textureManager used texture manager.
     * @return a simple texture representing the given value with the adapted format.
     */
    private Texture createScientificStyleSprite(double value, TextureManager textureManager) {
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
        mantissaTextEntity.setTextAntiAliased(true);
        mantissaTextEntity.setTextUseFractionalMetrics(axisProperty.getFontFractional());
        mantissaTextEntity.setTextColor(ColorFactory.createColor(colorMap, axisProperty.getFontColor()));
        mantissaTextEntity.setFont(mantissaFont);
        final Dimension mantissaSize = mantissaTextEntity.getSize();

        /**
         * Create exponent.
         */
        final TextEntity exponentTextEntity = new TextEntity(exponent.toString());
        Font exponentFont = FontManager.getSciFontManager().getFontFromIndex(axisProperty.getFontStyle(), axisProperty.getFontSize() * EXPONENT_SIZE_RATIO);
        exponentTextEntity.setTextAntiAliased(true);
        exponentTextEntity.setTextUseFractionalMetrics(axisProperty.getFontFractional());
        exponentTextEntity.setTextColor(ColorFactory.createColor(colorMap, axisProperty.getFontColor()));
        exponentTextEntity.setFont(exponentFont);
        final Dimension exponentSize = exponentTextEntity.getSize();

        Texture texture = textureManager.createTexture();

        texture.setDrawer(new TextureDrawer() {

            @Override
            public void draw(TextureDrawingTools drawingTools) {
                drawingTools.draw(mantissaTextEntity, 0, exponentSize.height);
                drawingTools.draw(exponentTextEntity, mantissaSize.width, 0);
            }

            @Override
            public Dimension getTextureSize() {
                return new Dimension(
                           exponentSize.width + mantissaSize.width,
                           exponentSize.height + mantissaSize.height
                       );
            }

            @Override
            public TextureDrawer.OriginPosition getOriginPosition() {
                return TextureDrawer.OriginPosition.UPPER_LEFT;
            }
        });

        return texture;
    }

    /**
     * Create and return a simple texture representing the given value.
     * @param text the formatted string representing the value.
     * @param textureManager used texture manager.
     * @return a simple texture representing the given value with the adapted format.
     */
    private Texture createSimpleSprite(String text, TextureManager textureManager) {
        Font font = FontManager.getSciFontManager().getFontFromIndex(axisProperty.getFontStyle(), axisProperty.getFontSize());
        final TextEntity textEntity = new TextEntity(text);
        textEntity.setTextAntiAliased(true);
        textEntity.setTextUseFractionalMetrics(axisProperty.getFontFractional());
        textEntity.setTextColor(ColorFactory.createColor(colorMap, axisProperty.getFontColor()));
        textEntity.setFont(font);

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
            public TextureDrawer.OriginPosition getOriginPosition() {
                return TextureDrawer.OriginPosition.UPPER_LEFT;
            }
        });

        return texture;
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
        if (index == -1 || index >= axisProperty.getTicksLabels().size()) {
            return null;
        } else {
            return axisProperty.getTicksLabels().get(index);
        }
    }
}
