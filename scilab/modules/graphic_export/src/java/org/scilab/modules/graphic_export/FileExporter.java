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
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import org.scilab.modules.localization.Messages;

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
     * @return 0 if everything worked fine, a non null integer if an exception occurred
     *         depending on the kind of error
     */
    public static String fileExport(int figureUID, String fileName, int fileType, float jpegCompressionQuality, int fileOrientation, boolean headless) {
        int ret = Export.export(figureUID, fileType, fileName, new ExportParams(jpegCompressionQuality, fileOrientation, true), headless);

        switch (ret) {
            case Export.SUCCESS :
                return "";
            case Export.IOEXCEPTION_ERROR :
                return Messages.gettext("Unable to create export file, permission denied.");
            case Export.INVALID_FILE :
                return Messages.gettext("Unable to create export file, invalid file.");
            case Export.MEMORY_ERROR :
                return Messages.gettext("Unable to create export file, not enough memory. Decreasing the number of elements or the size of the figure should fix this error.");
            case Export.UNKNOWN_ERROR :
                return Messages.gettext("Unable to create export file, please fill a bug report at http://bugzilla.scilab.org.");
            case Export.FILENOTFOUND_ERROR :
                return Messages.gettext("Unable to create export file, the file cannot be created or is locked by an other process.");
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
     * @return 0 if everything worked fine, a non null integer if an exception occurred
     *         depending on the kind of error
     */
    public static String fileExport(int figureUID, String fileName, int fileType, float jpegCompressionQuality, int fileOrientation) {
        return fileExport(figureUID, fileName, fileType, jpegCompressionQuality, fileOrientation, false);
    }

    /**
     * Export a figure into a file
     * @param figureIndex index of the figure to export
     * @param fileName name of the file to create
     * @param fileType kind of the file
     * @param jpegCompressionQuality the JPEG compression quality
     * @param fileOrientation orientation of the file
     * @return 0 if everything worked fine, a non null integer if an exception occurred
     *         depending on the kind of error
     */
    public static String fileExport(int figureUID, String fileName, String fileType, float jpegCompressionQuality, int fileOrientation) {
        String ext = fileType.toLowerCase();
        return fileExport(figureUID, fileName, Export.getType(ext), jpegCompressionQuality, fileOrientation, false);
    }

    /**
     * Export a figure into a file
     * @param figureIndex index of the figure to export
     * @param fileName name of the file to create
     * @param fileType kind of the file
     * @param jpegCompressionQuality the JPEG compression quality
     * @param fileOrientation orientation of the file
     * @return 0 if everything worked fine, a non null integer if an exception occurred
     *         depending on the kind of error
     */
    public static String headlessFileExport(int figureUID, String fileName, String fileType, float jpegCompressionQuality, int fileOrientation) {
        String ext = fileType.toLowerCase();
        return fileExport(figureUID, fileName, Export.getType(ext), jpegCompressionQuality, fileOrientation, true);
    }
}
