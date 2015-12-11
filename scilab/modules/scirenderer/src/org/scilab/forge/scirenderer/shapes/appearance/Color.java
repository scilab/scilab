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

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public final class Color extends java.awt.Color {

    private static final float COMPONENT_MAX_VALUE = 255f;

    /**
     * The default color.
     */
    private static final Color DEFAULT_COLOR = new Color(.2f, .3f, .4f);

    /**
     * Default constructor.
     * Create a copy of the default color.
     */
    public Color() {
        this(DEFAULT_COLOR);
    }

    /**
     * Copy constructor
     * @param c the color to copy.
     */
    public Color(Color c) {
        super(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
    }

    /**
     * Creates an sRGB color with the specified red, green, blue, and
     * alpha values in the range [0.0; 1.0].  The actual color
     * used in rendering depends on finding the best match given the
     * color space available for a particular output device.
     *
     * @param r the red component
     * @param g the green component
     * @param b the blue component
     * @param a the alpha component
     */
    public Color(float r, float g, float b, float a) {
        super(r, g, b, a);
    }

    /**
     * Creates an opaque sRGB color with the specified red, green, and blue
     * values in the range [0.0; 1.0].  Alpha is defaulted to 1.0.  The
     * actual color used in rendering depends on finding the best
     * match given the color space available for a particular output
     * device.
     *
     * @param r the red component
     * @param g the green component
     * @param b the blue component
     */
    public Color(float r, float g, float b) {
        super(r, g, b);
    }

    /**
     * Return red component value. In the range [0; 1].
     * @return red component value. In the range [0; 1].
     */
    public float getRedAsFloat() {
        return ((float) getRed()) / COMPONENT_MAX_VALUE;
    }

    /**
     * Return green component value. In the range [0; 1].
     * @return green component value. In the range [0; 1].
     */
    public float getGreenAsFloat() {
        return ((float) getGreen()) / COMPONENT_MAX_VALUE;
    }

    /**
     * Return blue component value. In the range [0; 1].
     * @return blue component value. In the range [0; 1].
     */
    public float getBlueAsFloat() {
        return ((float) getBlue()) / COMPONENT_MAX_VALUE;
    }

    /**
     * Return alpha component value. In the range [0; 1].
     * @return alpha component value. In the range [0; 1].
     */
    public float getAlphaAsFloat() {
        return ((float) getAlpha()) / COMPONENT_MAX_VALUE;
    }
}
