/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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
import org.scilab.forge.scirenderer.sprite.SpriteDrawer;
import org.scilab.forge.scirenderer.sprite.SpriteDrawingTools;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.forge.scirenderer.sprite.TextEntity;
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
public class FormattedTextSpriteDrawer implements SpriteDrawer {
    private final TextEntity textEntity;
    private final Dimension dimension;
    private final Icon icon;

    public FormattedTextSpriteDrawer(ColorMap colorMap, SpriteManager spriteManager, FormattedText formattedText) {
        if (formattedText != null) {
            String text = formattedText.getText();
            Font font = formattedText.getFont();
            if (isLatex(text)) {
                LoadClassPath.loadOnUse("graphics_latex_textrendering");
                TeXFormula formula = new TeXFormula(text.substring(1, text.length() - 1));
                formula.setColor(ColorFactory.createColor(colorMap, font.getColor()));
                icon = formula.createTeXIcon(TeXConstants.STYLE_DISPLAY, FontManager.scilabSizeToAwtSize(font.getSize()));
            } else if (isMathML(text)) {
                icon = ScilabSpecialTextUtilities.compileMathMLExpression(text, ((int) FontManager.scilabSizeToAwtSize(font.getSize() + .5)));
            } else {
                icon = null;
            }

            if (icon != null) {
                textEntity = null;
                dimension = new Dimension(icon.getIconWidth(), icon.getIconHeight());
            } else {
                textEntity = new TextEntity(formattedText.getText());

                textEntity.setFont(FontManager.getSciFontManager().getFontFromIndex(font.getStyle(), font.getSize()));
                textEntity.setText(formattedText.getText());
                textEntity.setTextColor(ColorFactory.createColor(colorMap, font.getColor()));
                textEntity.setTextUseFractionalMetrics(font.getFractional());
                textEntity.setTextAntiAliased(false);

                dimension = spriteManager.getSize(textEntity);
            }
        } else {
            icon = null;
            textEntity = null;
            dimension = new Dimension();
        }
    }

    @Override
    public void draw(SpriteDrawingTools drawingTools) {
        if (textEntity != null) {
            drawingTools.draw(textEntity, 0, 0);
        } else if (icon != null) {
            drawingTools.draw(icon, 0, 0);
        }
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
    private boolean isLatex(String string) {
        return (string.length() >= 2) && string.endsWith("$") && string.startsWith("$");
    }

    /**
     * Return true if the given string represent a MathML entity.
     * @param string the given string.
     * @return true if the given string represent a MathML entity.
     */
    private boolean isMathML(String string) {
        return (string.length() >= 2) && string.endsWith(">") && string.startsWith("<");
    }
}
