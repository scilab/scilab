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

package org.scilab.forge.scirenderer.implementation.jogl.utils;

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;

import java.awt.BasicStroke;
import java.awt.Graphics2D;
import java.awt.Stroke;

/**
 *
 * Utility class for common {@link java.awt.Graphics2D} calls.
 *
 * @author Pierre Lando
 */
public final class G2DShortCuts {

    /**
     * Private constructor: this is an utility class.
     */
    private G2DShortCuts() {
    }



    /**
     * Use the given color for drawing.
     * @param g2d the {@link Graphics2D} where the color will be used.
     * @param c the given color.
     */
    public static void useColor(Graphics2D g2d, Color c) {
        java.awt.Color color = new java.awt.Color(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
        g2d.setColor(color);
    }

    /**
     * Use the given appearance for drawing lines.
     * @param g2d the {@link Graphics2D} where the appearance will be used.
     * @param appearance the given appearance.
     */
    public static void useLineAppearance(Graphics2D g2d, Appearance appearance) {
        // TODO : add line pattern.
        Stroke stroke = new BasicStroke(appearance.getLineWidth());
        g2d.setStroke(stroke);
    }
}
