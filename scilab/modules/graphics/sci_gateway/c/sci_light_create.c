/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "Light.h"
#include "stricmp.h"



int sci_light_create(char *fname, unsigned long fname_len)
{
    char * axes = NULL;
    int type = -1;
    BOOL visible = TRUE;
    double * position = NULL;
    double * direction = NULL;
    double * ambient_color = NULL;
    double * diffuse_color = NULL;
    double * specular_color = NULL;

    SciErr sciErr;
    int* piAddr	= NULL;
    int nbRow, nbCol;
    int * piBool;
    int length = 0;
    char str[16];
    char * pStr = str;
    char * light;
    long long light_handle;
    long long axes_hdl = -1;
    long long* pAxesHandle = &axes_hdl;
    BOOL result;


    static rhs_opts opts[] =
    {

        { -1, "ambient_color", -1, 0, 0, NULL},
        { -1, "diffuse_color", -1, 0, 0, NULL},
        { -1, "direction", -1, 0, 0, NULL},
        { -1, "position", -1, 0, 0, NULL},
        { -1, "specular_color", -1, 0, 0, NULL},
        { -1, "type", -1, 0, 0, NULL},
        { -1, "visible", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };


    if (nbInputArgument(pvApiCtx) == 0)
    {
        result = createLight(-1, -1, TRUE, NULL, NULL, NULL, NULL, NULL, &light_handle);
    }
    else
    {

        CheckInputArgument(pvApiCtx, 1, 15);


        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
            if (sciErr.iErr)
            {
                return 0;
            }
            getMatrixOfHandle(pvApiCtx, piAddr, &nbRow, &nbCol, &pAxesHandle);

            if (pAxesHandle == NULL) return FALSE;

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return FALSE;
            }
        }

        if (getOptionals(pvApiCtx, fname, opts) == 0)
        {
            ReturnArguments(pvApiCtx);
            return 0;
        }

        if ((opts[6].iPos != -1) && (opts[6].iType == sci_strings) && (opts[6].iCols * opts[6].iRows == 1))
        {
            sciErr = getMatrixOfString(pvApiCtx, opts[6].piAddr, &nbRow, &nbCol, &length, NULL);
            if (length < 15 && !sciErr.iErr)
            {
                sciErr = getMatrixOfString(pvApiCtx, opts[6].piAddr, &nbRow, &nbCol, &length, &pStr);
                if (stricmp(str, "on") == 0)
                {
                    visible = 1;
                }
                else if (stricmp(str, "off") == 0)
                {
                    visible = 0;
                }
            }
        }

        if ((opts[5].iPos != -1) && (opts[5].iType == sci_strings) && (opts[5].iCols * opts[5].iRows == 1))
        {
            sciErr = getMatrixOfString(pvApiCtx, opts[5].piAddr, &nbRow, &nbCol, &length, NULL);
            if (length < 15 && !sciErr.iErr)
            {
                sciErr = getMatrixOfString(pvApiCtx, opts[5].piAddr, &nbRow, &nbCol, &length, &pStr);
                if (stricmp(str, "directional") == 0)
                {
                    type = 0;
                }
                else if (stricmp(str, "point") == 0)
                {
                    type = 1;
                }
            }
        }

        if ((opts[3].iPos != -1) && (opts[3].iType == sci_matrix) && (opts[3].iCols * opts[3].iRows == 3))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, opts[3].piAddr, &nbRow, &nbCol, &position);
        }

        if ((opts[2].iPos != -1) && (opts[2].iType == sci_matrix) && (opts[2].iCols * opts[2].iRows == 3))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, opts[2].piAddr, &nbRow, &nbCol, &direction);
        }

        if ((opts[0].iPos != -1) && (opts[0].iType == sci_matrix) && (opts[0].iCols * opts[0].iRows == 3))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, opts[0].piAddr, &nbRow, &nbCol, &ambient_color);
        }

        if ((opts[1].iPos != -1) && (opts[1].iType == sci_matrix) && (opts[1].iCols * opts[1].iRows == 3))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, opts[4].piAddr, &nbRow, &nbCol, &diffuse_color);
        }

        if ((opts[4].iPos != -1) && (opts[4].iType == sci_matrix) && (opts[4].iCols * opts[4].iRows == 3))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, opts[4].piAddr, &nbRow, &nbCol, &specular_color);
        }

        result = createLight(*pAxesHandle, type, visible, position, direction, ambient_color, diffuse_color, specular_color, &light_handle);

    }

    if (result == FALSE) return FALSE;


    nbRow = 1;
    nbCol = 1;

    sciErr = createMatrixOfHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &light_handle);
    if (sciErr.iErr)
    {
        return FALSE;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;

}
