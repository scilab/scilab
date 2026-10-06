/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 * Copyright (C) 2024 - UTC - Stéphane MOTTELET
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
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

package org.scilab.modules.renderer.JoGLView.util;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.font.TextLayout;
import java.awt.geom.Rectangle2D;

import javax.swing.Icon;
import javax.swing.SwingUtilities;

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.texture.TextEntity;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.modules.console.utils.ScilabSpecialTextUtilities;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.textObject.TextObject;
import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.renderer.utils.textRendering.FontManager;

/**
 * A {@see SpriteDrawer} who draw a Scilab {@see Text} object.
 * @author Pierre Lando
 */
public class TextObjectSpriteDrawer implements TextureDrawer {

    /**
     * Scilab text margin.
     */
    private static final int HMARGIN = 2;
    private static final int VMARGIN = 2;
    private static final int SPACEWIDTH = (int) Math.ceil(new TextEntity("_").getSize().getWidth()) - 2;

    private Appearance appearance;
    private int thickness;
    private final Object[][] entities;
    private float alignmentFactor;

    private final int[] lineHeight;
    private final int[] columnWidth;
    private final float[] lineAscent;

    private int width;
    private int height;

    private boolean latexSet = false;
    private boolean mathmlSet = false;

    /**
     * Default constructor.
     * @param colorMap the color map to use.
     * @param textObject the scilab {@see Text} to draw.
     */
    public TextObjectSpriteDrawer(final ColorMap colorMap, final TextObject textObject) {
        Integer[] dimensions = textObject.getTextArrayDimensions();
        int columnNumber = dimensions[1];
        int lineNumber = dimensions[0];

        this.lineHeight = new int[lineNumber];
        this.lineAscent = new float[lineNumber];
        this.columnWidth = new int[columnNumber];
        this.entities = new Object[columnNumber][lineNumber];
    }

    /**
     * Constructor.
     * Specifies a scale factor used to scale the text matrix.
     * @param colorMap the color map to used.
     * @param textObject the scilab {@see TextObject} to draw.
     * @param scaleFactor the scale factor to apply.
     */
    public TextObjectSpriteDrawer(final ColorMap colorMap, final TextObject textObject, double scaleFactor) {
        Integer[] dimensions = textObject.getTextArrayDimensions();
        int columnNumber = dimensions[1];
        int lineNumber = dimensions[0];

        this.lineHeight = new int[lineNumber];
        this.lineAscent = new float[lineNumber];
        this.columnWidth = new int[columnNumber];
        this.entities = new Object[columnNumber][lineNumber];
    }

    /**
     * Finish initialization after construction (entity matrix filling would leak
     * this from the constructor).
     * Called once right after instantiation, before the drawer is used.
     * Must run after the subclass has configured appearance, alignment and thickness.
     * @param colorMap the color map to use
     * @param textObject the text object to draw
     * @param scaleFactor the scale factor to apply
     */
    public void init(final ColorMap colorMap, final TextObject textObject, double scaleFactor) {
        Integer[] dimensions = textObject.getTextArrayDimensions();
        int columnNumber = dimensions[1];
        int lineNumber = dimensions[0];

        boolean fractionalFont = textObject.getFontFractional();
        Color textColor = ColorFactory.createColor(colorMap, textObject.getFont().getColor(), textObject.getFont().getColorAlpha().floatValue());
        Font font = computeFont(textObject, scaleFactor);

        /* Fill the entity matrix */
        loadDeps(textObject);
        fillEntityMatrix(textObject, fractionalFont, textColor, font);

        this.width  = (int)((double)sum(columnWidth) + scaleFactor * (double)(HMARGIN * (columnNumber + 1)) + 2 * thickness + scaleFactor * (double)(SPACEWIDTH * (columnNumber - 1)));
        this.height = (int)((double)sum(lineHeight)  + scaleFactor * (double)(VMARGIN * (lineNumber + 1)) + 2 * thickness);
    }

    /**
     * Load JLaTeXMath or JEuclid if mandatory
     * @param stringArray the text to check
     */
    protected void loadDeps(final TextObject textObject) {
        loop: {
            String[] textStrings = textObject.getTextStrings();
            String[] textInterpreters = textObject.getTextInterpreters();            
            for (int i=0; i<textStrings.length; i++) {
                if (textStrings[i] != null) {
                    if (!latexSet && ((isLatex(textStrings[i]) && textInterpreters[i].equals("auto")) || textInterpreters[i].equals("latex")) ) {
                        latexSet = true;
                        LoadClassPath.loadOnUse("graphics_latex_textrendering");
                    } else if (!mathmlSet && ((isMathML(textStrings[i]) && textInterpreters[i].equals("auto")) || textInterpreters[i].equals("mathml")) ) {
                        LoadClassPath.loadOnUse("graphics_mathml_textrendering");
                    } else if (latexSet && mathmlSet) {
                        break loop;
                    }
                }
            }
        }
    }

    /**
     * Fills the entity matrix
     * @param stringArray the matrix of text strings used to fill the entity matrix.
     * @param fractionalFont specifies whether a fractional font is used or not.
     * @param textColor the text color.
     * @param font the font to use.
     */
    protected void fillEntityMatrix(final TextObject textObject, boolean fractionalFont, Color textColor, Font font) {
        Integer[] dimensions = textObject.getTextArrayDimensions();
        String[] textStrings = textObject.getTextStrings();
        String[] textInterpreters = textObject.getTextInterpreters();
        int columnNumber = dimensions[1];
        int lineNumber = dimensions[0];
        for (int line = 0; line < lineNumber; line++ ) {
            for (int column = 0; column < columnNumber; column++) {
                String text = textStrings[line + column*lineNumber];
                String interpreter = textInterpreters[line + column*lineNumber];
                boolean success = true;
                if (text != null) {
                    Dimension dimension = null;
                    Icon icon = null;
                    float ascent = 0;
                    if ((isLatex(text) && interpreter.equals("auto")) || interpreter.equals("latex")) {
                        try {
                            TeXFormula formula = new TeXFormula(formatLaTeXString(text));
                            formula.setColor(textColor);
                            icon = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, font.getSize());
                            ascent = ((TeXIcon) icon).getIconHeight() - ((TeXIcon) icon).getIconDepth();
                        } catch (Exception e) { 
                            success = false;
                        }
                    } else if ((isMathML(text) && interpreter.equals("auto")) || interpreter.equals("mathml")) {
                        try {
                            icon = ScilabSpecialTextUtilities.compileMathMLExpression(text, font.getSize(), textColor);
                            ScilabSpecialTextUtilities.SpecialIcon si = (ScilabSpecialTextUtilities.SpecialIcon) icon;
                            ascent = si.getIconHeight() - si.getIconDepth();
                        } catch (Exception e) {
                            success = false;
                        }
                    }

                    if (icon != null) {
                        dimension = new Dimension(icon.getIconWidth(), icon.getIconHeight());
                        entities[column][line] = icon;
                    } else {
                        TextEntity textEntity = new TextEntity(text);
                        textEntity.setTextUseFractionalMetrics(fractionalFont);
                        textEntity.setTextAntiAliased(true);
                        if (success) {
                            textEntity.setTextColor(textColor);
                        } else {
                            textEntity.setTextColor(new Color(1.0f, 0.f, 0.f));
                        }
                        textEntity.setFont(font);
                        entities[column][line] = textEntity;
                        dimension = textEntity.getSize();
                        ascent = textEntity.isValid() ? textEntity.getLayout().getAscent() : 0;
                    }

                    lineAscent[line] = Math.max(lineAscent[line], ascent);

                    if (dimension != null) {
                        columnWidth[column] = Math.max(columnWidth[column], dimension.width);
                        lineHeight[line] = Math.max(lineHeight[line], dimension.height);
                    }
                }
            }
        }
    }

    /**
     * Return true if the given string represent a latex entity.
     * @param string the given string.
     * @return true if the given string represent a latex entity.
     */
    private boolean isLatex(String string) {
        long count = string.chars().filter(ch -> ch == '$').count();
        return (string.length() >= 2) && (count==2) && string.endsWith("$") && string.startsWith("$");
    }


    /**
     * Format a mixed LaTeX string
     * @param string the given string.
     * @return the formatted string
     */
    public String formatLaTeXString(String string) {
        String fmtString = "";
        // string is splitted in pure math and text chunks
        if (isLatex(string))
        {
            fmtString = string.substring(1, string.length() - 1);
        }
        else
        {
            fmtString = "\\text{" + string + "}";
        }
        return fmtString;
    }

    /**
     * Return true if the given string represent a MathML entity.
     * @param string the given string.
     * @return true if the given string represent a MathML entity.
     */
    private boolean isMathML(String string) {
        return (string.length() >= 2) && string.endsWith(">") && string.startsWith("<");
    }

    @Override
    public void draw(TextureDrawingTools drawingTools) {
        // Draw background.
        if (appearance.getFillColor().getAlphaAsFloat() != 0) {
            drawingTools.clear(appearance.getFillColor());
        }

        final int currentHMargin = getHMargin();
        final int currentVMargin = getVMargin();
        final int currentSpaceWidth = getSpaceWidth();

        // Draw text.
        int x = currentHMargin + thickness;
        int column = 0;
        for (Object[] entitiesLine : entities) {
            int y = currentVMargin + thickness;
            int line = 0;
            for (Object entity : entitiesLine) {
                if (entity != null) {
                    if (entity instanceof TextEntity) {
                        TextEntity textEntity = (TextEntity) entity;
                        float ascent = textEntity.isValid() ? textEntity.getLayout().getAscent() : 0.f;
                        double deltaX = alignmentFactor * (columnWidth[column] - textEntity.getSize().getWidth());
                        drawingTools.draw(textEntity, (int) (x + deltaX), Math.round(y - ascent + lineAscent[line]));
                        y += lineHeight[line] + currentVMargin;
                        line++;
                    } else if (entity instanceof Icon) {
                        Icon icon = (Icon) entity;
                        double deltaX = alignmentFactor * (columnWidth[column] - icon.getIconWidth());
                        if (latexSet && (icon instanceof TeXIcon)) {
                            TeXIcon tex = (TeXIcon) icon;
                            float ascent = tex.getIconHeight() - tex.getIconDepth();
                            drawingTools.draw(icon, (int) (x + deltaX), Math.round(y - ascent + lineAscent[line]));
                        } else {
                            // MathML
                            ScilabSpecialTextUtilities.SpecialIcon si = (ScilabSpecialTextUtilities.SpecialIcon) icon;
                            int ascent = si.getIconHeight() - si.getIconDepth();
                            drawingTools.draw(icon, (int) (x + deltaX), y - ascent + Math.round(lineAscent[line]));
                        }
                        y += lineHeight[line] + currentVMargin;
                        line++;
                    }
                }
            }
            x += columnWidth[column] + currentHMargin + currentSpaceWidth;
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

    @Override
    public Dimension getTextureSize() {
        return new Dimension(width, height);
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

    public int getHMargin() {
        return HMARGIN;
    }

    public int getVMargin() {
        return VMARGIN;
    }

    public int getSpaceWidth() {
        return SPACEWIDTH;
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
     * Computes and returns the {@link Font} adapted to the given Scilab text, taking into account the scale factor.
     * It takes the size 1 Font to derive a new Font whose size is increased according to the scale factor.
     * @param text the given {@see Text} object.
     * @param scaleFactor the scale factor to apply.
     * @return the {@see Font} adapted to the given Scilab text.
     */
    private Font computeFont(final TextObject text, double scaleFactor) {
        Font font = FontManager.getSciFontManager().getFontFromIndex(text.getFontStyle(), 1.0);
        return font.deriveFont(font.getSize2D() * (float)scaleFactor);
    }

    /**
     * Compute and return the alignment factor corresponding to the given scilab text.
     * @param text the given scilab text.
     * @return the alignment factor corresponding to the given scilab text.
     */
    protected float computeAlignmentFactor(Text text) {
        switch (text.getAlignmentAsEnum()) {
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
