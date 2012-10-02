/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * desc : interface for xs2file routine
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "api_scilab.h"
#include "xs2file.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "IsAScalar.h"
#include "localization.h"
#include "SetJavaProperty.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "PATH_MAX.h"
#include "MALLOC.h"
#include "FigureList.h"
#include "freeArrayOfString.h"
#include "../../../graphics/src/c/getHandleProperty/getPropertyAssignedValue.h"
#include "HandleManagement.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
static BOOL isVectorialExport(ExportFileType fileType);
/*--------------------------------------------------------------------------*/
int xs2file(char * fname, ExportFileType fileType )
{
    int *piAddr1;
    int *piAddr2;
    int *piAddr3;
    int *piLen;
    int i;
    SciErr sciErr;
    int iType1 = 0;

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

    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }//

    sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (iType1 != sci_matrix && iType1 != sci_handles)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer or a handle expected.\n"), fname, 1);
        return 1;
    }

    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if ( (isStringType(pvApiCtx, piAddr2)) )
    {
        char **fileName = NULL;
        char *real_filename = NULL;
        float jpegCompressionQuality = 0.95f;
        ExportOrientation orientation = EXPORT_PORTRAIT; /* default orientation */
        int m1 = 0, n1 = 0, l1 = 0;
        double figurenum = -1;
        char* figureUID = NULL;
        char *status = NULL;


        /* get handle by figure number */
        if (iType1 == sci_matrix)
        {
            getScalarDouble(pvApiCtx, piAddr1, &figurenum);

            if (!sciIsExistingFigure(figurenum))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A valid figure_id expected.\n"), fname, 1);
                return 1;
            }
            figureUID = getFigureFromIndex(figurenum);
        }

        /* check given handle */
        if (iType1 == sci_handles)
        {
            int iHandleType = -1;
            int* piHandleType = &iHandleType;
            GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &m1, &n1, &l1);
            if (m1*n1 != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return 1;
            }

            figureUID = getObjectFromHandle(getHandleFromStack(l1));
            if (figureUID == NULL)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Graphic handle expected.\n"), fname, 1);
                return 1;
            }

            getGraphicObjectProperty(figureUID, __GO_TYPE__, jni_int, (void**)&piHandleType);

            if (iHandleType != __GO_FIGURE__)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"), fname, 1, "Figure");
                return 1;
            }
        }

        /* get file name */

        //get variable address
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }//MYMARK2

        //fisrt call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr2, &m1, &n1, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        piLen = (int*)malloc(sizeof(int) * m1 * n1);

        //second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr2, &m1, &n1, piLen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        fileName = (char**)malloc(sizeof(char*) * m1 * n1);
        for (i = 0 ; i < m1 * n1 ; i++)
        {
            fileName[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }

        //third call to retrieve data
        sciErr = getMatrixOfString(pvApiCtx, piAddr2, &m1, &n1, piLen, fileName);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }
        if (m1*n1 == 1)
        {
            if (nbInputArgument(pvApiCtx) == 3)
            {
                int nbCol = 0;
                int nbRow = 0;

                if (isVectorialExport(fileType))
                {

                    char **sciOrientation = NULL;

                    //get variable address
                    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }//MYMARK3

                    if (!isStringType(pvApiCtx, piAddr3))
                    {
                        freeArrayOfString(fileName, m1 * n1);
                        Scierror(999, _("%s: Wrong type for input argument #%d: Single character string expected.\n"), fname, 3);
                        return 1;
                    }


                    //fisrt call to retrieve dimensions
                    sciErr = getMatrixOfString(pvApiCtx, piAddr3, &nbRow, &nbCol, NULL, NULL);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    piLen = (int*)malloc(sizeof(int) * nbRow * nbCol);

                    //second call to retrieve length of each string
                    sciErr = getMatrixOfString(pvApiCtx, piAddr3, &nbRow, &nbCol, piLen, NULL);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    sciOrientation = (char**)malloc(sizeof(char*) * nbRow * nbCol);
                    for (i = 0 ; i < nbRow * nbCol ; i++)
                    {
                        sciOrientation[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
                    }

                    //third call to retrieve data
                    sciErr = getMatrixOfString(pvApiCtx, piAddr3, &nbRow, &nbCol, piLen, sciOrientation);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }
                    if (nbRow*nbCol == 1)
                    {
                        /* Value should be 'landscape' or 'portrait' but check only the first character */
                        /* for compatibility with Scilab 4*/
                        if (strcmp(sciOrientation[0], "landscape") == 0 || strcmp(sciOrientation[0], "l") == 0)
                        {
                            freeArrayOfString(sciOrientation, nbRow * nbCol);
                            orientation = EXPORT_LANDSCAPE;
                        }
                        else if (strcmp(sciOrientation[0], "portrait") == 0 || strcmp(sciOrientation[0], "p") == 0)
                        {
                            freeArrayOfString(sciOrientation, nbRow * nbCol);
                            orientation = EXPORT_PORTRAIT;
                        }
                        else
                        {
                            freeArrayOfString(fileName, m1 * n1);
                            freeArrayOfString(sciOrientation, nbRow * nbCol);
                            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 3, "portrait", "landscape");
                            return 1;
                        }
                    }
                    else
                    {
                        freeArrayOfString(fileName, m1 * n1);
                        freeArrayOfString(sciOrientation, nbRow * nbCol);
                        Scierror(999, _("%s: Wrong size for input argument #%d: Single character string expected.\n"), fname, 3);
                        return 1;
                    }
                }
                else
                {
                    double quality = 0;
                    //get variable address of the input argument
                    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    getScalarDouble(pvApiCtx, piAddr3, &quality);

                    if (quality < 0 || quality > 1)
                    {
                        freeArrayOfString(fileName, m1 * n1);
                        Scierror(999, _("%s: Wrong type for input argument #%d: A real between 0 and 1 expected.\n"), fname, 3);
                        return 1;
                    }
                    jpegCompressionQuality = (float) quality;
                }
            }

            /* Replaces SCI, ~, HOME, TMPDIR by the real path */
            real_filename = expandPathVariable(fileName[0]);

            /* Call the function for exporting file */
            status = exportToFile(figureUID, real_filename, fileType, jpegCompressionQuality, orientation);

            /* free pointers no more used */
            if (real_filename)
            {
                FREE(real_filename);
                real_filename = NULL;
            }
            freeArrayOfString(fileName, m1 * n1);

            /* treat errors */
            if (strlen(status) != 0)
            {
                Scierror(999, _("%s: %s\n"), fname, status);
                return 1;
            }
        }
        else
        {
            freeArrayOfString(fileName, m1 * n1);
            Scierror(999, _("%s: Wrong size for input argument #%d: Single character string expected.\n"), fname, 2);
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
static BOOL isVectorialExport(ExportFileType fileType)
{
    return fileType == EPS_EXPORT
           || fileType == PS_EXPORT
           || fileType == PDF_EXPORT
           || fileType == SVG_EXPORT;
}
/*--------------------------------------------------------------------------*/
