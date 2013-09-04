/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.modules.renderer.JoGLView.label;

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.JoGLView.util.TextObjectSpriteDrawer;

/**
 * A {@see SpriteDrawer} which draws a Scilab {@see Label} object.
 * To do:
 *    -take into account the font angle property
 *    -refactoring
 * @author Pierre Lando
 */
public class LabelSpriteDrawer extends TextObjectSpriteDrawer implements TextureDrawer {

    /**
    * Default constructor.
    * @param colorMap the color map to use.
    * @param label the scilab {@see Label} to draw.
    */
    public LabelSpriteDrawer(final ColorMap colorMap, final Label label) {
        super(colorMap, label);
        /* Alignment factor set to left */
        setAlignmentFactor(0.0f);

        Appearance appearance = computeAppearance(colorMap, label);
        setAppearance(appearance);
        setThickness((int) Math.ceil(appearance.getLineWidth()));
    }

    /**
     * Compute and return an {@see Appearance} adapted to the given scilab label.
     * @param colorMap the current scilab color map.
     * @param label the given label.
     * @return an appearance adapted to the given scilab label.
     */
    private Appearance computeAppearance(final ColorMap colorMap, final Label label) {
        Appearance a = new Appearance();

        if (label.getFillMode()) {
            Color fillColor = ColorFactory.createColor(colorMap, label.getBackground());
            a.setFillColor(fillColor);
        } else {
            a.setFillColor(new Color(0, 0, 0, 0));
        }

        /* Fill mode must be set to on */
        if (label.getLineMode() && label.getFillMode()) {
            Color lineColor = ColorFactory.createColor(colorMap, label.getLineColor());
            a.setLineColor(lineColor);
            a.setLineWidth(1);
        } else {
            a.setLineWidth(0);
        }

        return a;
    }

}
