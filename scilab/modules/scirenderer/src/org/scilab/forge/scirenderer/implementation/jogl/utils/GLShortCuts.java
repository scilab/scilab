/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
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

package org.scilab.forge.scirenderer.implementation.jogl.utils;

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;

import javax.media.opengl.GL2;

/**
 *
 * Utility class for common OpenGl calls.
 *
 * @author Pierre Lando
 */
public final class GLShortCuts {

    private static final short FULL_LINE_PATTERN = (short) 0xFFFF;


    /**
     * Private constructor.
     * This is an utility class.
     */
    private GLShortCuts() {
    }


    /**
     * Set the OpenGl context line appearance from the given appearance.
     * If appearance is null, default value are used..
     * @param gl the OpenGl context.
     * @param appearance  the appearance to use.
     */
    public static void useLineAppearance(GL2 gl, Appearance appearance) {
        Appearance usedAppearance;
        if (appearance == null) {
            usedAppearance = new Appearance();
        } else {
            usedAppearance = appearance;
        }

        useColor(gl, usedAppearance.getLineColor());
        gl.glLineWidth(getClampedLineWidth(gl, usedAppearance.getLineWidth()));
        short pattern = usedAppearance.getLinePattern();
        if (pattern != FULL_LINE_PATTERN) {
            gl.glEnable(GL2.GL_LINE_STIPPLE);
            gl.glLineStipple((int) usedAppearance.getLineWidth(), pattern);
        } else {
            gl.glDisable(GL2.GL_LINE_STIPPLE);
        }
    }

    /**
     * Return given lineWidth clamped to lineWidth range.
     * @param gl the current gl context.
     * @param lineWidth given line width.
     * @return given lineWidth clamped to lineWidth range.
     */
    private static float getClampedLineWidth(GL2 gl, float lineWidth) {
        float[] range = new float[] {1f, 1f};
        if (gl.glIsEnabled(GL2.GL_LINE_SMOOTH)) {
            gl.glGetFloatv(GL2.GL_SMOOTH_LINE_WIDTH_RANGE, range, 0);
        } else {
            gl.glGetFloatv(GL2.GL_ALIASED_LINE_WIDTH_RANGE, range, 0);
        }

        if (lineWidth < range[0]) {
            return range[0];
        } else if (lineWidth > range[1]) {
            return range[1];
        } else {
            return lineWidth;
        }
    }

    /**
     * Set the OpenGl context color to the given color.
     * @param gl the OpenGl context.
     * @param color the color to use.
     */
    public static void useColor(GL2 gl, Color color) {
        if (color != null) {
            gl.glColor3f(color.getRedAsFloat(), color.getGreenAsFloat(), color.getBlueAsFloat());
        }
    }

    /**
     * Enable or disable GL option.
     * @param gl the current gl.
     * @param option the option to change.
     * @param status the new option status.
     */
    public static void setEnable(GL2 gl, int option, boolean status) {
        if (status) {
            gl.glEnable(option);
        } else {
            gl.glDisable(option);
        }
    }
}
