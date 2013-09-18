/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.g2d.motor;

import java.awt.BasicStroke;
import java.awt.Stroke;
import java.util.HashMap;
import java.util.Map;

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;

/**
 * @author Calixte DENIZET
 */
public class G2DStroke extends BasicStroke {

    private static final int[] array = new int[16];
    private static final Stroke BASIC = new G2DStroke(1, null, 0);

    public G2DStroke(float lineWidth, float[] dash, float phase) {
        super(lineWidth, CAP_BUTT, JOIN_MITER, 10.0f, dash, phase);
    }

    public static Stroke getStroke(Appearance appearance, double dashPhase) {
        Appearance usedAppearance;
        if (appearance == null) {
            usedAppearance = new Appearance();
        } else {
            usedAppearance = appearance;
        }

        float factor = usedAppearance.getLineWidth();
        if (factor == 0) {
            return new G2DStroke(0, null, 0);
        }

        short pattern = usedAppearance.getLinePattern();
        if (pattern == -1 ) {
            if (factor == 1) {
                return BASIC;
            }

            return new G2DStroke(factor, null, 0);
        }

        return new G2DStroke(factor, decodePattern(factor, pattern), (float) dashPhase);
    }

    private static final float[] decodePattern(final float factor, short pattern) {
        // If the pattern is 1111101011111010, from right to left it becomes
        // 0101111101011111, the first 0 is put on the right 1011111010111110 and it is converted into
        // 1, 1, 5, 1, 1, 1, 5, 1
        int n = 0xFFFF & pattern;
        int i = 0;
        int t = Integer.numberOfTrailingZeros(n);
        n = (n >> t);
        int k = Integer.numberOfLeadingZeros(n) - 16;
        while (n != 0) {
            t = Integer.numberOfTrailingZeros(n);
            if (t == 0) {
                t = Integer.numberOfTrailingZeros(0xFFFF - n);
            }

            array[i++] = t;
            n = n >> t;
        }
        array[i] = k;

        float[] ret = new float[i + 1];

        for (int j = 0; j <= i; j++) {
            ret[j] = ((float) array[j]) * factor;
        }

        return ret;
    }
}
