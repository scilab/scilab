/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.shapes.appearance;

import org.scilab.forge.scirenderer.texture.Texture;

/**
 *
 * Basic class for appearance parameters.
 *
 * @author Pierre Lando
 */
public final class Appearance {

    /**
     * The default line width.
     */
    public static final float DEFAULT_LINE_WIDTH = 1.0f;

    /**
     * The default line pattern (full line).
     * The 16 bits of the pattern represents how lines are drawn.
     */
    public static final short DEFAULT_LINE_PATTERN = (short) 0xFFFF;

    /**
     * The default line color.
     */
    public static final Color DEFAULT_LINE_COLOR = new Color(.8f, .8f, .8f);

    /**
     * The default fill color.
     */
    public static final Color DEFAULT_FILL_COLOR = new Color(.8f, .8f, .8f);

    /**
     * Current {@link Texture}
     */
    private Texture texture;

    /**
     * The current line width.
     */
    private float lineWidth = DEFAULT_LINE_WIDTH;

    /**
     * The current line pattern.
     * The 16 bits of the pattern represents how lines are drawn.
     */
    private short linePattern = DEFAULT_LINE_PATTERN;

    /**
     * the current line color.
     */
    private Color lineColor = DEFAULT_LINE_COLOR;

    /**
     * The current fill color.
     */
    private Color fillColor = DEFAULT_FILL_COLOR;

    /**
     * The material used for lighting.
     */
    private Material material;

    /**
     * Default constructor.
     */
    public Appearance() {
    }

    /**
     * Return the default appearance.
     * @return the default appearance.
     */
    public static Appearance getDefault() {
        return new Appearance();
    }

    /**
     * Texture getter.
     * @return the current texture if any.
     */
    public Texture getTexture() {
        if ((texture != null) && (texture.isValid())) {
            return texture;
        } else {
            return null;
        }
    }

    /**
     * Texture setter.
     * @param texture the new texture.
     */
    public void setTexture(Texture texture) {
        this.texture = texture;
    }

    /**
     * Return the line width.
     * @return the line width.
     */
    public float getLineWidth() {
        return lineWidth;
    }

    /**
     * Set the line width.
     * @param lineWidth the new line width.
     */
    public void setLineWidth(float lineWidth) {
        this.lineWidth = lineWidth;
    }

    /**
     * Return the line pattern.
     * @return the line pattern.
     */
    public short getLinePattern() {
        return linePattern;
    }

    /**
     * Set the line pattern.
     * @param linePattern the new line pattern.
     */
    public void setLinePattern(short linePattern) {
        this.linePattern = linePattern;
    }

    /**
     * Return the line color.
     * @return the line color.
     */
    public Color getLineColor() {
        return lineColor;
    }

    /**
     * Set the line color.
     * @param lineColor the new lne color.
     */
    public void setLineColor(Color lineColor) {
        this.lineColor = lineColor;
    }

    /**
     * Return the fill color.
     * @return the fill color.
     */
    public Color getFillColor() {
        return fillColor;
    }

    /**
     * Set the fill color.
     * @param fillColor the new fill color.
     */
    public void setFillColor(Color fillColor) {
        this.fillColor = fillColor;
    }

    /**
     * Get the material.
     * @return  the material.
     */
    public Material getMaterial() {
        return material;
    }

    /**
     * Set the material.
     * @param  the new material.
     */
    public void setMaterial(Material m) {
        material = m;
    }
}

