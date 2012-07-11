/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.legend;

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.JoGLView.util.TextObjectSpriteDrawer;

/**
 * A {@see SpriteDrawer} who draw a Scilab {@see Legend} object.
 * Adapted from TextSpriteDrawer, minus the unneeded functionalities.
 * (text box, etc.).
 * To do: refactoring.
 * @author Pierre Lando
 */
public class LegendSpriteDrawer extends TextObjectSpriteDrawer implements TextureDrawer {

    /**
     * Default constructor.
     * @param colorMap the color map to use.
     * @param legend the scilab {@see Legend} to draw.
     */
    LegendSpriteDrawer(final ColorMap colorMap, final Legend legend) {
        super(colorMap, legend);
        Appearance appearance = computeAppearance(colorMap, legend);
        setAppearance(appearance);
        setThickness((int) Math.ceil(appearance.getLineWidth()));
    }

    /**
     * Compute and return an {@see Appearance} adapted to the given scilab legend.
     * @param colorMap the current scilab color map.
     * @param legend the given legend.
     * @return an appearance adapted to the given scilab legend.
     */
    private Appearance computeAppearance(final ColorMap colorMap, final Legend legend) {
        Appearance a = new Appearance();

        if (legend.getFillMode()) {
            Color fillColor = ColorFactory.createColor(colorMap, legend.getBackground());
            a.setFillColor(fillColor);
        } else {
            a.setFillColor(new Color(0, 0, 0, 0));
        }

        /* Force line width to 0 */
        /* For legends, line mode is used to draw the legend outline */
        /* Line mode must not be used for the text */
        a.setLineWidth(0);

        return a;
    }


    /**
     * Compute and return the matrix of text string from the given {@see Legend} object.
     * The legend text array is treated as a 1D-array.
     * @param legend the given {@see Legend} object.
     * @return the matrix of text strings from the given {@see Legend} object.
     */
    private String[][] computeTextData(final Legend legend) {
        String[] textString = legend.getTextStrings();
        Integer[] dimensions = legend.getTextArrayDimensions();

        String[][] texts = new String[legend.getLinks().length][1];

        /* Number of actually displayed text items */
        int nbTextItems = Math.min(legend.getLinks().length, textString.length);

        for (int i = 0; i < nbTextItems; i++) {
            texts[i][0] = textString[i];
        }

        /* Pad with blank strings if less input Strings than links */
        if (nbTextItems < legend.getLinks().length) {
            for (int i = nbTextItems; i < legend.getLinks().length; i++) {
                texts[i][0] = new String(" ");
            }
        }

        return texts;
    }


}
