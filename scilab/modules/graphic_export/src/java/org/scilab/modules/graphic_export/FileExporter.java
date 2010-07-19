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

import java.io.File;
import java.io.IOException;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;

/**
 * Static class used to create file export of graphic figures
 * @author Jean-Baptiste Silvy
 */
public class FileExporter {

    /** Export waiting message */
    private static final String exportingMessage = "Exporting figure, please wait...";

    /** The id used on classpath.xml to load vectorial export JARs */
    private static final String CLASSPATH_PDF_PS_EPS_EXPORT_NAME = "pdf_ps_eps_graphic_export";

    /**
     * Default constructor
     */

    protected FileExporter() {
    }

    /**
     * Export a figure into a file
     * @param figureIndex index of the figure to export
     * @param fileName name of the file to create
     * @param fileType kind of the file
     * @param fileOrientation orientation of the file
     * @return 0 if everything worked fine, a non null integer if an exception occured
     *         depending on the kind of error
     */
    public static int fileExport(int figureIndex, String fileName, int fileType, int fileOrientation) {
        int saveFileType = -1;
        String saveFileName = "";
    
        DrawableFigureGL exportedFig = FigureMapper.getCorrespondingFigure(figureIndex);

        if (exportedFig == null) {
            // figure no longer exists
            return ExportRenderer.IOEXCEPTION_ERROR;
        }

        //When the graphic-export is too long, we inform the user that the figure is exporting
        String oldInfoMessage = exportedFig.getInfoMessage();
        exportedFig.setInfoMessage(exportingMessage);
        if (fileType == ExportRenderer.PDF_EXPORT || fileType == ExportRenderer.EPS_EXPORT || fileType == ExportRenderer.PS_EXPORT ) {

            /* Under !Windows, make sure that the library for ps export
             * are already loaded
             * Note that this code is an ugly workaround to avoid the explicit call
             * to:
             * LoadClassPath.loadOnUse(CLASSPATH_PDF_PS_EPS_EXPORT_NAME);
             * which creates a cyclic dependencies on:
             *  graphic_export => jvm => gui => graphic_export
             * This code will retrieve on the fly the object and call the method
             */
            try {
                Class jvmLoadClassPathClass = Class.forName("org.scilab.modules.jvm.LoadClassPath");
                Method loadOnUseMethod = jvmLoadClassPathClass.getDeclaredMethod("loadOnUse", new Class[] { String.class });
                loadOnUseMethod.invoke(null, CLASSPATH_PDF_PS_EPS_EXPORT_NAME);
            } catch (java.lang.ClassNotFoundException ex) {
                System.err.println("Could not find the Scilab class to load the export dependencies: " + ex);
            } catch (java.lang.NoSuchMethodException ex) {
                System.err.println("Could not find the Scilab method to load the export dependencies: " + ex);
            } catch (java.lang.IllegalAccessException ex) {
                System.err.println("Could not access to the Scilab method to load the export dependencies: " + ex);
            } catch (java.lang.reflect.InvocationTargetException ex) {
                System.err.println("Could not invoke the Scilab method to load the export dependencies: " + ex);
            }

            String ext = "";

            switch (fileType) {
                case ExportRenderer.PDF_EXPORT:
                    ext = ".pdf";
                    break;
                case ExportRenderer.EPS_EXPORT:
                    ext = ".eps";
                    break;
                case ExportRenderer.PS_EXPORT:
                    ext = ".ps";
                    break;
                default: /* Do not the extension. Probably an error */
                    return ExportRenderer.IOEXCEPTION_ERROR;
            }

            String name = new File(fileName).getName();
            int dotPosition = name.lastIndexOf(".");
            if (dotPosition > 0) {
                name = name.substring(0, dotPosition);
                saveFileName = fileName.substring(0, fileName.lastIndexOf(".")) + ext;
            } else {
                saveFileName = fileName + ext;
            }

            try {
                /* Temporary SVG file which will be used to convert to PDF */
                fileName = File.createTempFile(name,".svg").getAbsolutePath();
            } catch (IOException e) {
                System.err.println("Could not create temporary file " + e.getLocalizedMessage());
            }

            saveFileType = fileType;
            fileType = ExportRenderer.SVG_EXPORT;
        }
    
        ExportRenderer export;
        export = ExportRenderer.createExporter(figureIndex, fileName, fileType, fileOrientation);

        // To be sure that their is a GLContext active for export
        exportedFig.openGraphicCanvas();

        exportedFig.getRenderingTarget().addGLEventListener(export);
        exportedFig.drawCanvas();
        exportedFig.getRenderingTarget().removeGLEventListener(export);

        //Put back the old infoMessage
        exportedFig.setInfoMessage(oldInfoMessage);

        if (saveFileType != -1) {
            ConvertSVG.SVGTo(fileName, saveFileName, saveFileType);
            new File(fileName).delete();
        }

        return ExportRenderer.getErrorNumber();
    }
}
