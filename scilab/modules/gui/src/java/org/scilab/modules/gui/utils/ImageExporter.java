/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Image;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvas;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;


/**
 * Class specialized in exporting graphic rendering into an image.
 * @author Jean-Baptiste Silvy
 */
public final class ImageExporter {

    /**
     * Default constructor
     */
    protected ImageExporter() {

    }

    /**
     * Export a figure into an Image.
     * Might be used to copy it into a file or clipboard
     * @param figureIndex index of the figure to export
     * @return copy of the OpenGL canvas of the figure or null if an error occurred
     */
    public static Image imageExport(Integer uid) {
        DrawerVisitor visitor = DrawerVisitor.getVisitor(uid);
        Canvas canvas = visitor.getCanvas();
        if (canvas instanceof JoGLCanvas) {
            JoGLCanvas joglCanvas = (JoGLCanvas) canvas;
            return joglCanvas.getImage();
        }

        return null;
    }

}
