/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
