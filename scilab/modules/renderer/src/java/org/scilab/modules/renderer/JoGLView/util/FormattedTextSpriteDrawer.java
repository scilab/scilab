/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.texture.TextEntity;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.modules.console.utils.ScilabSpecialTextUtilities;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.textObject.Font;
import org.scilab.modules.graphic_objects.textObject.FormattedText;
import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.renderer.utils.textRendering.FontManager;

import javax.swing.Icon;
import java.awt.Dimension;

/**
 * @author Pierre Lando
 */
public class FormattedTextSpriteDrawer implements TextureDrawer {
    private TextEntity textEntity;
    private Dimension dimension;
    private int descent;
    private Icon icon = null;

    public FormattedTextSpriteDrawer(ColorMap colorMap, FormattedText formattedText) {
        String text = formattedText.getText();
        String interpreter = FormattedText.InterpreterType.enumToString(formattedText.getInterpreterAsEnum());
        Font font = formattedText.getFont();
        boolean success = true;
        if (text != null && font != null) {
            if ((isLatex(text) && interpreter.equals("auto")) || interpreter.equals("latex")) {
                LoadClassPath.loadOnUse("graphics_latex_textrendering");
                try {
                    TeXFormula formula = new TeXFormula(formatLaTeXString(text));
                    formula.setColor(ColorFactory.createColor(colorMap, font.getColor(), font.getColorAlpha().floatValue()));
                    icon = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, FontManager.scilabSizeToAwtSize(font.getSize()));
                    descent = ((TeXIcon) icon).getIconDepth();                    
                } catch (Exception e) {
                    success = false;
                }
            } else if ((isMathML(text) && interpreter.equals("auto")) || interpreter.equals("mathml")) {
                LoadClassPath.loadOnUse("graphics_mathml_textrendering");
                try {
                    icon = ScilabSpecialTextUtilities.compileMathMLExpression(text, ((int) FontManager.scilabSizeToAwtSize(font.getSize() + .5)), 
                            ColorFactory.createColor(colorMap, font.getColor(), font.getColorAlpha().floatValue()));
                    descent = 0;
                } catch (Exception e) {
                    success = false;                    
                }
            } else {
                descent = 0;
            }

            if (icon != null) {
                textEntity = null;
                dimension = new Dimension(icon.getIconWidth(), icon.getIconHeight() + descent);
            } else {
                textEntity = new TextEntity(text);
                textEntity.setFont(FontManager.getSciFontManager().getFontFromIndex(font.getStyle(), font.getSize()));
                textEntity.setText(text);
                if (success) {
                    textEntity.setTextColor(ColorFactory.createColor(colorMap, font.getColor(), font.getColorAlpha().floatValue()));
                } else {
                    textEntity.setTextColor(new Color(1.0f, 0.f, 0.f));
                }
                textEntity.setTextUseFractionalMetrics(font.getFractional());
                textEntity.setTextAntiAliased(true);
                dimension = textEntity.getSize();
            }
        } else {
            icon = null;
            textEntity = null;
            dimension = new Dimension();
            descent = 0;
        }
    }

    @Override
    public void draw(TextureDrawingTools drawingTools) {
        if (textEntity != null) {
            drawingTools.draw(textEntity, 0, 0);
        } else if (icon != null) {
            drawingTools.draw(icon, 0, descent);
        }
    }

    @Override
    public Dimension getTextureSize() {
        return new Dimension(dimension);
    }

    @Override
    public OriginPosition getOriginPosition() {
        return OriginPosition.UPPER_LEFT;
    }

    public Dimension getSpriteSize() {
        return new Dimension(dimension);
    }

    /**
     * Return true if the given string represent a latex entity.
     * @param string the given string.
     * @return true if the given string represent a latex entity.
     */
    public boolean isLatex(String string) {
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
    public static boolean isMathML(String string) {
        return (string.length() >= 2) && string.endsWith(">") && string.startsWith("<");
    }
}
