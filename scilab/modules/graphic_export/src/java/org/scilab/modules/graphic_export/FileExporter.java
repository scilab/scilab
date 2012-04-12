/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - Calixte Denizet
 * desc : Static class used to create file export of graphic figures
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import java.lang.reflect.Method;

import java.awt.Color;
import java.io.File;
import java.io.IOException;
import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.g2d.G2DCanvasFactory;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvasFactory;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;

import java.awt.Graphics2D;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

import java.io.*;
import org.apache.batik.svggen.SVGGeneratorContext;
import org.apache.batik.svggen.SVGGraphics2D;
import org.apache.batik.dom.GenericDOMImplementation;

import org.w3c.dom.DOMImplementation;
import org.w3c.dom.Document;

/**
 * Static class used to create file export of graphic figures
 * @author Jean-Baptiste Silvy
 */
public class FileExporter {

    /** Export waiting message */
    private static final String exportingMessage = "Exporting figure, please wait...";

    /**
     * Default constructor
     */
    protected FileExporter() { }

    /**
     * Export a figure into a file
     * @param figureIndex index of the figure to export
     * @param fileName name of the file to create
     * @param fileType kind of the file
     * @param jpegCompressionQuality the JPEG compression quality
     * @param fileOrientation orientation of the file
     * @return 0 if everything worked fine, a non null integer if an exception occured
     *         depending on the kind of error
     */
    public static String fileExport(String figureUID, String fileName, int fileType, float jpegCompressionQuality, int fileOrientation) {
        int ret = Export.export(figureUID, fileType, fileName, new ExportParams(jpegCompressionQuality, fileOrientation, false));

        //int ret = Export.exportBitmap(figureUID, fileType, fileName, false, new ExportParams(jpegCompressionQuality, fileOrientation, false));

        switch (ret) {
        case Export.SUCCESS :
            return "";
        case Export.IOEXCEPTION_ERROR :
            return "Unable to create export file, permission denied.";
        case Export.INVALID_FILE :
            return "Unable to create export file, invalid file.";
        default :
            return "";
        }
    }

    /**
     * Export a figure into a file
     * @param figureIndex index of the figure to export
     * @param fileName name of the file to create
     * @param fileType kind of the file
     * @param jpegCompressionQuality the JPEG compression quality
     * @param fileOrientation orientation of the file
     * @return 0 if everything worked fine, a non null integer if an exception occured
     *         depending on the kind of error
     */
    public static String fileExport(String figureUID, String fileName, String fileType, float jpegCompressionQuality, int fileOrientation) {
        String ext = fileType.toLowerCase();
        return fileExport(figureUID, fileName, Export.getType(ext), jpegCompressionQuality, fileOrientation);
    }
}
