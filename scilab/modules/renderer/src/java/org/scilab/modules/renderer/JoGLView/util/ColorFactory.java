/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.util;

import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.modules.graphic_objects.figure.ColorMap;

/**
 * Utility class to create color form scilab color.
 *
 * @author Pierre Lando
 */
public final class ColorFactory {

    /**
     * Create a {@see Color} object representing the given scilab color.
     * @param colorMap the scilab colormap to use.
     * @param colorIndex the given scilab color.
     * @return a {@see Color} object representing the given scilab color.
     */
    public static Color createColor(ColorMap colorMap, int colorIndex) {
        if (colorMap != null) {
            float[] colorData = colorMap.getScilabColor(colorIndex);
            return new Color(colorData[0], colorData[1], colorData[2]);
        } else {
            return new Color(0, 0, 0);
        }
    }

    /**
     * Create a float array filled with the RGBA data of the given scilab color.
     * @param colorMap the scilab colormap to use.
     * @param colorIndex the given scilab color.
     * @return a float array filled with the RGBA data of the given scilab color.
     */
    public static float[] createRGBAColor(ColorMap colorMap, int colorIndex) {
        float[] rgbData = colorMap.getScilabColor(colorIndex);
        float[] rgbaData = new float[4];
        System.arraycopy(rgbData, 0, rgbaData, 0, 3);
        rgbaData[3] = 1f;
        return rgbaData;
    }
}
