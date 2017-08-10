/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - Calixte Denizet
 * desc : Static class used to create file export of graphic figures
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
