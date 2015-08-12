/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "BuildObjects.h"
#include "stricmp.h"



int sci_light(char *fname, unsigned long fname_len)
{
    int type = -1;
    BOOL visible = 1;
    double* position = NULL;
    double* direction = NULL;
    double* ambient_color = NULL;
    double* diffuse_color = NULL;
    double* specular_color = NULL;

    SciErr sciErr;
    int* piAddr	= NULL;
    int length = 0;
    char * pStr = NULL;
    int light = 0;
    int axes = 0;
    long long axesHandle = 0;

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
        light = ConstructLight(fname, 0, -1, TRUE, NULL, NULL, NULL, NULL, NULL);
    }
    else
    {
        CheckInputArgument(pvApiCtx, 1, 15);

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isHandleType(pvApiCtx, piAddr))
        {
            if (isScalar(pvApiCtx, piAddr))
            {
                if (getScalarHandle(pvApiCtx, piAddr, &axesHandle))
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle expected.\n"), fname, 1);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return 0;
            }

            axes = getObjectFromHandle((long)axesHandle);
        }

        if (getOptionals(pvApiCtx, fname, opts) == 0)
        {
            ReturnArguments(pvApiCtx);
            return 0;
        }

        if (opts[6].iPos != -1 && opts[6].iType == sci_strings && opts[6].iCols == 1 && opts[6].iRows == 1)
        {
            if (getAllocatedSingleString(pvApiCtx, opts[6].piAddr, &pStr))
            {
                if (pStr)
                {
                    freeAllocatedSingleString(pStr);
                }

                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 7);
                return 0;
            }

            if (stricmp(pStr, "on") == 0)
            {
                visible = 1;
            }
            else if (stricmp(pStr, "off") == 0)
            {
                visible = 0;
            }

            freeAllocatedSingleString(pStr);
        }

        if (opts[5].iPos != -1 && opts[5].iType == sci_strings && opts[5].iCols == 1 && opts[5].iRows == 1)
        {
            if (getAllocatedSingleString(pvApiCtx, opts[5].piAddr, &pStr))
            {
                if (pStr)
                {
                    freeAllocatedSingleString(pStr);
                }

                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 6);
                return 0;
            }

            if (stricmp(pStr, "directional") == 0)
            {
                type = 0;
            }
            else if (stricmp(pStr, "point") == 0)
            {
                type = 1;
            }

            freeAllocatedSingleString(pStr);
        }

        if ((opts[3].iPos != -1) && (opts[3].iType == sci_matrix) && (opts[3].iCols * opts[3].iRows == 3))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, opts[3].piAddr, &opts[3].iCols, &opts[3].iRows, &position);
        }

        if ((opts[2].iPos != -1) && (opts[2].iType == sci_matrix) && (opts[2].iCols * opts[2].iRows == 3))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, opts[2].piAddr, &opts[2].iCols, &opts[2].iRows, &direction);
        }

        if ((opts[0].iPos != -1) && (opts[0].iType == sci_matrix) && (opts[0].iCols * opts[0].iRows == 3))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, opts[0].piAddr, &opts[0].iCols, &opts[0].iRows, &ambient_color);
        }

        if ((opts[1].iPos != -1) && (opts[1].iType == sci_matrix) && (opts[1].iCols * opts[1].iRows == 3))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, opts[1].piAddr, &opts[1].iCols, &opts[1].iRows, &diffuse_color);
        }

        if ((opts[4].iPos != -1) && (opts[4].iType == sci_matrix) && (opts[4].iCols * opts[4].iRows == 3))
        {
            sciErr = getMatrixOfDouble(pvApiCtx, opts[4].piAddr, &opts[4].iCols, &opts[4].iRows, &specular_color);
        }

        light = ConstructLight(fname, axes, type, visible, position, direction, ambient_color, diffuse_color, specular_color);

    }

    //error occurs in ConstructLight
    if (light == 0)
    {
        //error is manage in ConstructLight
        return 0;
    }

    if (createScalarHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, getHandle(light)))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
