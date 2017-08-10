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
package org.scilab.modules.renderer.JoGLView.util;

import static org.scilab.modules.graphic_objects.figure.Figure.PixelDrawingMode;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLPixelDrawingMode;

public class PixelDrawingModeUtils {

    public static JoGLPixelDrawingMode figureToJoGLmode(PixelDrawingMode mode) {
        switch (mode) {
            case CLEAR:
                return JoGLPixelDrawingMode.CLEAR;
            case AND:
                return JoGLPixelDrawingMode.AND;
            case ANDREVERSE:
                return JoGLPixelDrawingMode.AND_REVERSE;
            case COPY:
                return JoGLPixelDrawingMode.COPY;
            case ANDINVERTED:
                return JoGLPixelDrawingMode.AND_INVERTED;
            case NOOP:
                return JoGLPixelDrawingMode.NOOP;
            case XOR:
                return JoGLPixelDrawingMode.XOR;
            case OR:
                return JoGLPixelDrawingMode.OR;
            case NOR:
                return JoGLPixelDrawingMode.NOR;
            case EQUIV:
                return JoGLPixelDrawingMode.EQUIV;
            case INVERT:
                return JoGLPixelDrawingMode.INVERT;
            case ORREVERSE:
                return JoGLPixelDrawingMode.OR_REVERSE;
            case COPYINVERTED:
                return JoGLPixelDrawingMode.COPY_INVERTED;
            case ORINVERTED:
                return JoGLPixelDrawingMode.OR_INVERTED;
            case NAND:
                return JoGLPixelDrawingMode.NAND;
            case SET:
                return JoGLPixelDrawingMode.SET;
            default:
                return  null;

        }
    }
}
