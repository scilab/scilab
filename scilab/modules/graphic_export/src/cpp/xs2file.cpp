/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * desc : interface for xs2file routine
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern "C"
{
#include "xs2file.h"
#include "api_scilab.h"
#include "GetProperty.h"
#include "localization.h"
#include "SetJavaProperty.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "PATH_MAX.h"
#include "sci_malloc.h"
#include "FigureList.h"
#include "freeArrayOfString.h"
#include "../../../graphics/src/c/getHandleProperty/getPropertyAssignedValue.h"
#include "HandleManagement.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

/*--------------------------------------------------------------------------*/
static bool isVectorialExport(ExportFileType fileType);
/*--------------------------------------------------------------------------*/
int xs2file(char * fname, ExportFileType fileType, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    int* piAddrfileName = NULL;
    int* piAddrsciOrientation = NULL;
    int* piAddrquality = NULL;
    double* quality = NULL;

    /* Check input and output sizes */
    CheckOutputArgument(pvApiCtx, 0, 1);
    if (isVectorialExport(fileType) || fileType == JPG_EXPORT)
    {
        CheckInputArgument(pvApiCtx, 2, 3);
    }
    else
    {
        CheckInputArgument(pvApiCtx, 2, 2);
    }

    if ((!checkInputArgumentType(pvApiCtx, 1, sci_matrix)) && (!checkInputArgumentType(pvApiCtx, 1, sci_handles)))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer or a handle expected.\n"), fname, 1);
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 1;
    }

    if ( (checkInputArgumentType(pvApiCtx, 2, sci_strings)) )
    {
        char **fileName = NULL;
        char *real_filename = NULL;
        float jpegCompressionQuality = 0.95f;
        ExportOrientation orientation = EXPORT_PORTRAIT; /* default orientation */
        int m1 = 0, n1 = 0;
        int figurenum = -1;
        int figureUID = 0;
        char *status = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        /* get handle by figure number */
        if (checkInputArgumentType(pvApiCtx, 1, sci_matrix))
        {
            // Retrieve a matrix of double at position 1.
            int* l1 = NULL;
            sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrl1, &m1, &n1, &l1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            if (m1 * n1 != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
                return 1;
            }

            figurenum = *l1;
            if (!sciIsExistingFigure(figurenum))
            {
                Scierror(999, "%s: Input argument #%d must be a valid figure_id.\n", fname, 1);
                return 1;
            }
            figureUID = getFigureFromIndex(figurenum);
        }
        /* check given handle */
        else if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            int iHandleType = -1;
            int* piHandleType = &iHandleType;
            long long* l1 = NULL;

            // Retrieve a matrix of handle at position 1.
            sciErr = getMatrixOfHandle(pvApiCtx, piAddrl1, &m1, &n1, &l1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: Handle matrix expected.\n"), fname, 1);
                return 1;
            }

            if (m1 * n1 != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return 1;
            }

            figureUID = getObjectFromHandle((unsigned long) * l1);
            if (figureUID == 0)
            {
                Scierror(999, _("%s: Input argument #%d must be a valid handle.\n"), fname, 1);
                return 1;
            }

            getGraphicObjectProperty(figureUID, __GO_TYPE__, jni_int, (void**)&piHandleType);

            if (iHandleType != __GO_FIGURE__)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"), fname, 1, "Figure");
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar or figure handle expected.\n"), fname, 1);
            return 1;
        }

        /* get file name */
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrfileName);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of string at position 2.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrfileName, &m1, &n1, &fileName))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
            return 1;
        }

        if (m1 * n1 == 1)
        {
            if (nbInputArgument(pvApiCtx) == 3)
            {
                int nbCol = 0;
                int nbRow = 0;

                if (isVectorialExport(fileType))
                {

                    char **sciOrientation = NULL;

                    if ((!checkInputArgumentType(pvApiCtx, 3, sci_strings)))
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: Single character string expected.\n"), fname, 3);
                        freeAllocatedMatrixOfString(m1, n1, fileName);
                        return 1;
                    }

                    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrsciOrientation);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        freeAllocatedMatrixOfString(m1, n1, fileName);
                        return 1;
                    }

                    // Retrieve a matrix of string at position 3.
                    if (getAllocatedMatrixOfString(pvApiCtx, piAddrsciOrientation, &nbRow, &nbCol, &sciOrientation))
                    {
                        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
                        freeAllocatedMatrixOfString(m1, n1, fileName);
                        return 1;
                    }

                    if (nbRow * nbCol == 1)
                    {
                        /* Value should be 'landscape' or 'portrait' but check only the first character */
                        /* for compatibility with Scilab 4*/
                        if (strcmp(sciOrientation[0], "landscape") == 0 || strcmp(sciOrientation[0], "l") == 0)
                        {
                            freeAllocatedMatrixOfString(nbRow, nbCol, sciOrientation);
                            orientation = EXPORT_LANDSCAPE;
                        }
                        else if (strcmp(sciOrientation[0], "portrait") == 0 || strcmp(sciOrientation[0], "p") == 0)
                        {
                            freeAllocatedMatrixOfString(nbRow, nbCol, sciOrientation);
                            orientation = EXPORT_PORTRAIT;
                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 3, "portrait", "landscape");
                            freeAllocatedMatrixOfString(m1, n1, fileName);
                            freeAllocatedMatrixOfString(nbRow, nbCol, sciOrientation);
                            return 1;
                        }
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: Single character string expected.\n"), fname, 3);
                        freeAllocatedMatrixOfString(m1, n1, fileName);
                        freeAllocatedMatrixOfString(nbRow, nbCol, sciOrientation);
                        return 1;
                    }
                }
                else
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrquality);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        freeAllocatedMatrixOfString(m1, n1, fileName);
                        return 1;
                    }

                    // Retrieve a matrix of double at position 3.
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddrquality, &nbRow, &nbCol, &quality);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
                        freeAllocatedMatrixOfString(m1, n1, fileName);
                        return 1;
                    }

                    if (nbRow != 1 || nbCol != 1 || *quality < 0 || *quality > 1)
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: A real between 0 and 1 expected.\n"), fname, 3);
                        freeAllocatedMatrixOfString(m1, n1, fileName);
                        return 1;
                    }
                    jpegCompressionQuality = (float) * quality;
                }
            }

            /* Replaces SCI, ~, HOME, TMPDIR by the real path */
            real_filename = expandPathVariable(fileName[0]);
            freeAllocatedMatrixOfString(m1, n1, fileName);

            /* Call the function for exporting file */
            status = exportToFile(figureUID, real_filename, fileType, jpegCompressionQuality, orientation);

            /* free pointers no more used */
            if (real_filename)
            {
                FREE(real_filename);
                real_filename = NULL;
            }

            /* treat errors */
            if (strlen(status) != 0)
            {
                Scierror(999, _("%s: %s\n"), fname, status);
                delete[] status;
                return 1;
            }

            delete[] status;
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Single character string expected.\n"), fname, 2);
            freeAllocatedMatrixOfString(m1, n1, fileName);
            return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Single character string expected.\n"), fname, 2);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
static bool isVectorialExport(ExportFileType fileType)
{
    return fileType == EPS_EXPORT
           || fileType == PS_EXPORT
           || fileType == PDF_EXPORT
           || fileType == SVG_EXPORT
           || fileType == EMF_EXPORT;
}
/*--------------------------------------------------------------------------*/
