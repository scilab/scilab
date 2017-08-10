/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Functions to export files
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

#include "exportToFile.hxx"
#include "FileExporter.hxx"


extern "C"
{
#include "getScilabJavaVM.h"
#include "GetProperty.h"
}



/*---------------------------------------------------------------------------------*/
char * exportToFile(int iFigureUID,
                    const char * fileName,
                    ExportFileType fileType,
                    const float jpegCompressionQuality,
                    ExportOrientation orientation)
{
    return org_scilab_modules_graphic_export::FileExporter::fileExport(getScilabJavaVM(),
            iFigureUID,
            (char *)fileName,
            fileType,
            jpegCompressionQuality,
            orientation);
}
/*---------------------------------------------------------------------------------*/
