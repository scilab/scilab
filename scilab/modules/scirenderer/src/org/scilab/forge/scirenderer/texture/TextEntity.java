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

package org.scilab.forge.scirenderer.texture;

import org.scilab.forge.scirenderer.shapes.appearance.Color;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.font.FontRenderContext;
import java.awt.font.TextLayout;
import java.awt.geom.Rectangle2D;

/**
 * @author Pierre Lando
 */
public class TextEntity {

    /**
     * The default text color.
     */
    public static final Color DEFAULT_TEXT_COLOR = new Color(0.f, 0.f, 0.f);

    /**
     * The default text anti-aliased status.
     */
    public static final boolean DEFAULT_TEXT_ANTI_ALIASED = true;

    /**
     * The default text use fractional matrix status.
     */
    public static final boolean DEFAULT_TEXT_USE_FRACTIONAL_METRICS = true;

    /**
     * The default font.
     */
    private static final Font DEFAULT_FONT = new Font(null);

    /**
     * The current text color.
     */
    private Color textColor = DEFAULT_TEXT_COLOR;

    /**
     * The current text anti-aliased status.
     */
    private boolean textAntiAliased = DEFAULT_TEXT_ANTI_ALIASED;

    /**
     * The current text use fractional matrix status.
     */
    private boolean textUseFractionalMetrics = DEFAULT_TEXT_USE_FRACTIONAL_METRICS;

    /**
     * The text content of this object.
     */
    private String text;

    /**
     * The font used by this text entity.
     */
    private Font font;

    private TextLayout layout;

    /**
     * Default constructor.
     * @param text the text content.
     */
    public TextEntity(String text) {
        this.text = text;
        this.font = DEFAULT_FONT;
    }

    /**
     * Return the text content.
     * @return the text content.
     */
    public String getText() {
        return text;
    }

    /**
     * Set the text content.
     * @param text the new text content.
     */
    public void setText(String text) {
        this.text = text;
        this.layout = null;
    }

    /**
     * Return the text font.
     * @return the text font.
     */
    public Font getFont() {
        return font;
    }

    /**
     * Set the text font.
     * @param font the new text font.
     */
    public void setFont(Font font) {
        this.font = font;
        this.layout = null;
    }

    /**
     * Return the text color.
     * @return the text color.
     */
    public Color getTextColor() {
        return textColor;
    }

    /**
     * Set the text color.
     * @param textColor the new text color.
     */
    public void setTextColor(Color textColor) {
        this.textColor = textColor;
    }

    /**
     * Return the text anti-aliased status.
     * @return the text anti-aliased status.
     */
    public boolean isTextAntiAliased() {
        return textAntiAliased;
    }

    /**
     * Set the text anti-aliased status.
     * @param textAntiAliased the new text anti-aliased status.
     */
    public void setTextAntiAliased(boolean textAntiAliased) {
        this.textAntiAliased = textAntiAliased;
        this.layout = null;
    }

    /**
     * Return the text use fractional metrics status.
     * @return the text use fractional metrics status.
     */
    public boolean isTextUseFractionalMetrics() {
        return textUseFractionalMetrics;
    }

    /**
     * Set the text use fractional metrics status.
     * @param textUseFractionalMetrics the new text use fractional metrics status.
     */
    public void setTextUseFractionalMetrics(boolean textUseFractionalMetrics) {
        this.textUseFractionalMetrics = textUseFractionalMetrics;
        this.layout = null;
    }

    /**
     * TextEntity validity getter.
     * @return true if the text entity is valid.
     */
    public boolean isValid() {
        return ((getFont() != null)
                && (getText() != null)
                && (getText().length() > 0)
               );
    }

    public TextLayout getLayout() {
        if (layout == null) {
            FontRenderContext frc = new FontRenderContext(null, isTextAntiAliased(), isTextUseFractionalMetrics());
            layout = new TextLayout(getText(), getFont(), frc);
        }

        return layout;
    }

    /**
     * Return the dimension in pixel of the text entity.
     * @return the dimension in pixel of the text entity.
     */
    public Dimension getSize() {
        if (isValid()) {
            TextLayout textLayout = getLayout();
            Dimension dimension = new Dimension();
            Rectangle2D r = textLayout.getBounds();
            /* +1 added to fix rendering of ticks labels, a pixel row/column was missing */
            dimension.setSize(r.getWidth() + 2, textLayout.getAscent() + textLayout.getDescent() + 1);
            return dimension;
        } else {
            return new Dimension(0, 0);
        }
    }
}
