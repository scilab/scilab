/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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
    private final TextEntity textEntity;
    private final Dimension dimension;
    private final int descent;
    private final Icon icon;

    public FormattedTextSpriteDrawer(ColorMap colorMap, String text, Font font) {
        if (text != null && font != null) {
            if (isLatex(text)) {
                LoadClassPath.loadOnUse("graphics_latex_textrendering");
                TeXFormula formula = new TeXFormula(text.substring(1, text.length() - 1));
                formula.setColor(ColorFactory.createColor(colorMap, font.getColor()));
                icon = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, FontManager.scilabSizeToAwtSize(font.getSize()));
                descent = ((TeXIcon) icon).getIconDepth();
            } else if (isMathML(text)) {
                LoadClassPath.loadOnUse("graphics_mathml_textrendering");
                icon = ScilabSpecialTextUtilities.compileMathMLExpression(text, ((int) FontManager.scilabSizeToAwtSize(font.getSize() + .5)), ColorFactory.createColor(colorMap, font.getColor()));
                descent = 0;
            } else {
                icon = null;
                descent = 0;
            }

            if (icon != null) {
                textEntity = null;
                dimension = new Dimension(icon.getIconWidth(), icon.getIconHeight() + descent);
            } else {
                textEntity = new TextEntity(text);

                textEntity.setFont(FontManager.getSciFontManager().getFontFromIndex(font.getStyle(), font.getSize()));
                textEntity.setText(text);
                textEntity.setTextColor(ColorFactory.createColor(colorMap, font.getColor()));
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

    public FormattedTextSpriteDrawer(ColorMap colorMap, FormattedText formattedText) {
        this(colorMap, formattedText != null ? formattedText.getText() : null, formattedText != null ? formattedText.getFont() : null);
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
    public static boolean isLatex(String string) {
        return (string.length() >= 2) && string.endsWith("$") && string.startsWith("$");
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
