/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.legend;

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.sprite.SpriteDrawer;
import org.scilab.forge.scirenderer.sprite.SpriteDrawingTools;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.forge.scirenderer.sprite.TextEntity;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.utils.textRendering.FontManager;

import java.awt.Dimension;
import java.awt.Font;

/**
 * A {@see SpriteDrawer} who draw a Scilab {@see Legend} object.
 * Adapted from TextSpriteDrawer, minus the unneeded functionalities.
 * (text box, etc.).
 * To do: refactoring.
 * @author Pierre Lando
 */
public class LegendSpriteDrawer implements SpriteDrawer {

    /**
     * Scilab text margin.
     */
    private static final int MARGIN = 2;

    private final SpriteManager spriteManager;
    private final Appearance appearance;
    private final int thickness;
    private final TextEntity[][] entities;
    private final float alignmentFactor = 0.0f;
    private final int spaceWidth;

    private final int[] lineHeight;
    private final int[] columnWidth;

    private final int width;
    private final int height;

    /**
     * Default constructor.
     * @param spriteManager the sprite manager to use.
     * @param colorMap the color map to use.
     * @param legend the scilab {@see Legend} to draw.
     */
    LegendSpriteDrawer(final SpriteManager spriteManager, final ColorMap colorMap, final Legend legend) {
        this.spriteManager = spriteManager;
        this.appearance = computeAppearance(colorMap, legend);
        this.thickness = (int) Math.ceil(appearance.getLineWidth());
        this.spaceWidth = computeSpaceWidth();

        String[][] stringArray = computeTextData(legend);
        int columnNumber = -1;
        for (String[] stringLine : stringArray) {
            columnNumber = Math.max(stringLine.length, columnNumber);
        }
        int lineNumber = stringArray.length;

        this.lineHeight = new int[lineNumber];
        this.columnWidth = new int[columnNumber];
        this.entities = new TextEntity[columnNumber][lineNumber];

        boolean fractionalFont = legend.getFontFractional();
        Color textColor = ColorFactory.createColor(colorMap, legend.getFont().getColor());
        Font font = computeFont(legend);

        int line = 0;
        for (String[] textLine : stringArray) {
            int column = 0;
            for (String text : textLine) {
                if (text != null) {
                    TextEntity textEntity = new TextEntity(text);
                    textEntity.setTextUseFractionalMetrics(fractionalFont);
                    textEntity.setTextAntiAliased(false);
                    textEntity.setTextColor(textColor);
                    textEntity.setFont(font);
                    entities[column][line] = textEntity;
                    Dimension dimension = spriteManager.getSize(textEntity);
                    columnWidth[column] = Math.max(columnWidth[column], dimension.width);
                    lineHeight[line] = Math.max(lineHeight[line], dimension.height);
                }
                column++;
            }
            line++;
        }

        this.width  = sum(columnWidth) + MARGIN * (columnNumber + 1) + 2 * thickness + spaceWidth * (columnNumber - 1);
        this.height = sum(lineHeight)  + MARGIN * (lineNumber + 1) + 2 * thickness;
    }

    @Override
    public void draw(SpriteDrawingTools drawingTools) {
        // Draw text.
        int x = MARGIN + thickness;
        int column = 0;
        for (TextEntity[] entitiesLine : entities) {
            int y = MARGIN + thickness;
            int line = 0;
            for (TextEntity entity : entitiesLine) {
                double deltaX = alignmentFactor * (columnWidth[column] - spriteManager.getSize(entity).getWidth());
                if (entity != null) {
                    drawingTools.draw(entity, (int) (x + deltaX), y);
                }
                y += lineHeight[line] + MARGIN;
                line++;
            }
            x += columnWidth[column] + MARGIN + spaceWidth;
            column++;
        }
    }

    @Override
    public OriginPosition getOriginPosition() {
        return OriginPosition.UPPER_LEFT;
    }

    /**
     * Return the sprite width needed by this drawer.
     * @return the sprite width needed by this drawer.
     */
    public int getWidth() {
        return width;
    }

    /**
     * Return the sprite height needed by this drawer.
     * @return the sprite height needed by this drawer.
     */
    public int getHeight() {
        return height;
    }

    /**
     * Return the text margin.
     * @return the text margin.
     */
    public int getMargin() {
        return MARGIN;
    }

    /**
     * Compute and return an {@see Appearance} adapted to the given scilab legend.
     * @param colorMap the current scilab color map.
     * @param legend the given legend.
     * @return an appearance adapted to the given scilab legend.
     */
    private Appearance computeAppearance(final ColorMap colorMap, final Legend legend) {
        Appearance a = new Appearance();

        if (legend.getFillMode()) {
            Color fillColor = ColorFactory.createColor(colorMap, legend.getBackground());
            a.setFillColor(fillColor);
        } else {
            a.setFillColor(new Color(0, 0, 0, 0));
        }

        if (legend.getLineMode()) {
            Color lineColor = ColorFactory.createColor(colorMap, legend.getLineColor());
            a.setLineColor(lineColor);
            a.setLineWidth(1);
        } else {
            a.setLineWidth(0);
        }

        return a;
    }


    /**
     * Compute and return the matrix of text string from the given {@see Legend} object.
     * The legend text array is treated as a 1D-array.
     * @param legend the given {@see Legend} object.
     * @return the matrix of text strings from the given {@see Legend} object.
     */
    private String[][] computeTextData(final Legend legend) {
        String[] textString = legend.getTextStrings();
        Integer[] dimensions = legend.getTextArrayDimensions();

        String[][] texts = new String[legend.getLinks().length][1];

        /* Number of actually displayed text items */
        int nbTextItems = Math.min(legend.getLinks().length, textString.length);

        for (int i = 0; i < nbTextItems; i++) {
            texts[i][0] = textString[i];
        }

        /* Pad with blank strings if less input Strings than links */
        if (nbTextItems < legend.getLinks().length) {
            for (int i = nbTextItems; i < legend.getLinks().length; i++) {
                texts[i][0] = new String(" ");
            }
        }

        return texts;
    }

    /**
     * Compute and return the {@see Font} adapted to the given scilab legend.
     * @param legend the given scilab legend.
     * @return the {@see Font} adapted to the given scilab legend.
     */
    private Font computeFont(final Legend legend) {
        return FontManager.getSciFontManager().getFontFromIndex(legend.getFontStyle(), legend.getFontSize());
    }

    /**
     * Compute and return the width of the space character.
     * @return the width of the space character.
     */
    private int computeSpaceWidth() {
        TextEntity spaceText = new TextEntity("_");
        return (int) Math.ceil(spriteManager.getSize(spaceText).getWidth());
    }

    /**
     * Util function.
     * Return sum of the element of the given array.
     * @param values the given array.
     * @return sum of the element of the given array.
     */
    private int sum(int[] values) {
        int sum = 0;
        for (int value : values) {
            sum += value;
        }
        return sum;
    }
}
