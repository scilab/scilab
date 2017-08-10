/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
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

package org.scilab.forge.scirenderer.implementation.g2d.motor;

import java.awt.BasicStroke;

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;

/**
 * @author Calixte DENIZET
 */
public class G2DStroke extends BasicStroke {

    private static final int[] array = new int[16];
    private static final G2DStroke BASIC = new G2DStroke(1, null, 0);
    private static float[] prevArray;
    private static float prevFactor = -1;
    private static short prevPattern = -1;

    public G2DStroke(float lineWidth, float[] dash, float phase) {
        super(lineWidth, CAP_BUTT, JOIN_MITER, 10.0f, dash, phase);
    }

    public static G2DStroke getStroke(Appearance appearance, double dashPhase) {
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

        if (factor != prevFactor || pattern != prevPattern) {
            prevFactor = factor;
            prevPattern = pattern;
            prevArray = decodePattern(factor, pattern);
        }

        return new G2DStroke(factor, prevArray, (float) dashPhase);
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

        // Here we try to have a shorter pattern (EPS and PS don't like too long pattern in setdash function)
        // If the pattern is 101101101101, we can factorize the number by 101 (101+101x1000+101x1000000+101x1000000000)
        // so we test if the pattern can be divided by 101010...., by 100100100..., ...)
        int twopm1 = (1 << 16) - 1;
        for (int j = 2; 2 <= 16; j++) {
            int q = twopm1 / ((1 << j) - 1);
            if (n % q == 0) {
                n = n / q;
                break;
            }
        }

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
