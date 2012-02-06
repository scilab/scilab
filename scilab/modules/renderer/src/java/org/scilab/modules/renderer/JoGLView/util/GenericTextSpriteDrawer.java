/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.util;

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.sprite.SpriteDrawer;
import org.scilab.forge.scirenderer.sprite.SpriteDrawingTools;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.forge.scirenderer.sprite.TextEntity;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.textObject.TextObject;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.utils.textRendering.FontManager;

import javax.swing.Icon;
import java.awt.Dimension;
import java.awt.Font;

/**
 * A {@see SpriteDrawer} who draw a Scilab {@see Text} object.
 * @author Pierre Lando
 */
public class GenericTextSpriteDrawer implements SpriteDrawer {

    /**
     * Scilab text margin.
     */
    private static final int MARGIN = 2;

    private final SpriteManager spriteManager;
    private Appearance appearance;
    private int thickness;
    private final Object[][] entities;
    private float alignmentFactor;
    private final int spaceWidth;

    private final int[] lineHeight;
    private final int[] columnWidth;

    private final int width;
    private final int height;

    /**
     * Default constructor.
     * @param spriteManager the sprite manager to use.
     * @param colorMap the color map to use.
     * @param textObject the scilab {@see Text} to draw.
     */
    public GenericTextSpriteDrawer(final SpriteManager spriteManager, final ColorMap colorMap, final TextObject textObject) {
        this.spriteManager = spriteManager;
        this.spaceWidth = computeSpaceWidth();

        String[][] stringArray = computeTextData(textObject);
        int columnNumber = -1;
        for (String[] stringLine : stringArray) {
            columnNumber = Math.max(stringLine.length, columnNumber);
        }
        int lineNumber = stringArray.length;

        this.lineHeight = new int[lineNumber];
        this.columnWidth = new int[columnNumber];
        this.entities = new Object[columnNumber][lineNumber];

        boolean fractionalFont = textObject.getFontFractional();
        Color textColor = ColorFactory.createColor(colorMap, textObject.getFont().getColor());
        Font font = computeFont(textObject);

        int line = 0;
        for (String[] textLine : stringArray) {
            int column = 0;
            for (String text : textLine) {
                if (text != null) {
                    Dimension dimension = null;
                    if (isLatex(text)) {
                        TeXFormula formula = new TeXFormula(text.substring(1, text.length() - 1));
                        TeXIcon icon = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, font.getSize());
                        dimension = new Dimension(icon.getIconWidth(), icon.getIconHeight());
                        entities[column][line] = icon;
                    } else {
                        TextEntity textEntity = new TextEntity(text);
                        textEntity.setTextUseFractionalMetrics(fractionalFont);
                        textEntity.setTextAntiAliased(false);
                        textEntity.setTextColor(textColor);
                        textEntity.setFont(font);
                        entities[column][line] = textEntity;
                        dimension = spriteManager.getSize(textEntity);
                    }

                    if (dimension != null) {
                        columnWidth[column] = Math.max(columnWidth[column], dimension.width);
                        lineHeight[line] = Math.max(lineHeight[line], dimension.height);
                    }
                }
                column++;
            }
            line++;
        }

        this.width  = sum(columnWidth) + MARGIN * (columnNumber + 1) + 2 * thickness + spaceWidth * (columnNumber - 1);
        this.height = sum(lineHeight)  + MARGIN * (lineNumber + 1) + 2 * thickness;
    }

    /**
     * Return true if the given string represent a latex entity.
     * @param string the given string.
     * @return true if the given string represent a latex entity.
     */
    private boolean isLatex(String string) {
        return (string.length() >= 2) && string.endsWith("$") && string.startsWith("$");
    }

    /**
     * Return true if the given string represent a MathML entity.
     * @param string the given string.
     * @return true if the given string represent a MathML entity.
     */
    private boolean isMathML(String string) {
        return (string.length() >= 2) && string.endsWith("<") && string.startsWith(">");
    }

    @Override
    public void draw(SpriteDrawingTools drawingTools) {
        // Draw background.
        if (appearance.getFillColor().getAlphaAsFloat() != 0) {
            drawingTools.clear(appearance.getFillColor());
        }

        // Draw text.
        int x = MARGIN + thickness;
        int column = 0;
        for (Object[] entitiesLine : entities) {
            int y = MARGIN + thickness;
            int line = 0;
            for (Object entity : entitiesLine) {
                if (entity != null) {
                    if (entity instanceof TextEntity) {
                        TextEntity textEntity = (TextEntity) entity;
                        double deltaX = alignmentFactor * (columnWidth[column] - spriteManager.getSize(textEntity).getWidth());
                        drawingTools.draw(textEntity, (int) (x + deltaX), y);
                        y += lineHeight[line] + MARGIN;
                        line++;
                    } else if (entity instanceof Icon) {
                        Icon icon = (Icon) entity;
                        double deltaX = alignmentFactor * (columnWidth[column] - icon.getIconWidth());
                        drawingTools.draw(icon, (int) (x + deltaX), y);
                        y += lineHeight[line] + MARGIN;
                        line++;
                    }
                }
            }
            x += columnWidth[column] + MARGIN + spaceWidth;
            column++;
        }

        // Draw border lines.
        if (appearance.getLineWidth() > 0) {
            float hlw = appearance.getLineWidth() / 2;
            int x1 = (int) hlw;
            int y1 = (int) hlw;
            int x2 = (int) (width - hlw);
            int y2 = (int) (height - hlw);
            drawingTools.drawPolyline(new int[] {x1, y1, x2, y1, x2, y2, x1, y2, x1, y1}, appearance);
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

    protected void setAlignmentFactor(float alignmentFactor) {
        this.alignmentFactor = alignmentFactor;
    }

    protected void setAppearance(Appearance appearance) {
        this.appearance = appearance;
    }

    protected void setThickness(int thickness) {
        this.thickness = thickness;
    }

    public int getMargin() {
        return MARGIN;
    }
    
    /**
     * Compute and return the matrix of text string from the given {@see Text} object.
     * @param text the given {@see Text} object.
     * @return the matrix of text string from the given {@see Text} object.
     */
    protected String[][] computeTextData(final TextObject text) {
        String[] textString = text.getTextStrings();
        Integer[] dimensions = text.getTextArrayDimensions();
        String[][] texts = new String[dimensions[0]][dimensions[1]];
        int i = 0;
        for (int c = 0; c < dimensions[1]; c++) {
            for (int l = 0; l < dimensions[0]; l++) {
                texts[l][c] = textString[i];
                i++;
            }
        }
        return texts;
    }

    /**
     * Compute and return the {@see Font} adapted to the given scilab text.
     * @param text the given scilab text.
     * @return the {@see Font} adapted to the given scilab text.
     */
    private Font computeFont(final TextObject text) {
        return FontManager.getSciFontManager().getFontFromIndex(text.getFontStyle(), text.getFontSize());
    }

    /**
     * Compute and return the alignment factor corresponding to the given scilab text.
     * @param text the given scilab text.
     * @return the alignment factor corresponding to the given scilab text.
     */
    protected float computeAlignmentFactor(Text text) {
        switch(text.getAlignmentAsEnum()) {
        case LEFT:
            return 0f;
        case CENTER:
            return 1f / 2f;
        case RIGHT:
            return 1f;
        default:
            return 0f;
        }
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
