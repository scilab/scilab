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

package org.scilab.forge.scirenderer.implementation.g2d;

import java.awt.Graphics2D;

/**
 * @author Pierre Lando
 */
public final class G2DCanvasFactory {

    /**
     * Private constructor.
     * This is an utility class.
     */
    private G2DCanvasFactory() {

    }

    /**
     * Create a canvas from an auto drawable object.
     * @param autoDrawable the auto drawable object.
     * @return a canvas based on the given auto drawable object.
     */
    public static G2DCanvas createCanvas(Graphics2D g2d, int width, int height) {
        return new G2DCanvas(g2d, width, height);
    }
}
