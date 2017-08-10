/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2016 - Caio SOUZA
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
package org.scilab.forge.scirenderer.implementation.jogl;

public enum JoGLPixelDrawingMode {
    CLEAR, AND, AND_REVERSE, COPY, AND_INVERTED, NOOP, XOR, OR, NOR, EQUIV, INVERT, OR_REVERSE, COPY_INVERTED, OR_INVERTED, NAND, SET;

    /**
     * Converts an integer to the corresponding enum
     * @param intValue the integer value
     * @return  the pixel drawing mode enum
     */
    public static JoGLPixelDrawingMode intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return JoGLPixelDrawingMode.CLEAR;
            case 1:
                return JoGLPixelDrawingMode.AND;
            case 2:
                return JoGLPixelDrawingMode.AND_REVERSE;
            case 3:
                return JoGLPixelDrawingMode.COPY;
            case 4:
                return JoGLPixelDrawingMode.AND_INVERTED;
            case 5:
                return JoGLPixelDrawingMode.NOOP;
            case 6:
                return JoGLPixelDrawingMode.XOR;
            case 7:
                return JoGLPixelDrawingMode.OR;
            case 8:
                return JoGLPixelDrawingMode.NOR;
            case 9:
                return JoGLPixelDrawingMode.EQUIV;
            case 10:
                return JoGLPixelDrawingMode.INVERT;
            case 11:
                return JoGLPixelDrawingMode.OR_REVERSE;
            case 12:
                return JoGLPixelDrawingMode.COPY_INVERTED;
            case 13:
                return JoGLPixelDrawingMode.OR_INVERTED;
            case 14:
                return JoGLPixelDrawingMode.NAND;
            case 15:
                return JoGLPixelDrawingMode.SET;
            default:
                return  null;
        }
    }
};
