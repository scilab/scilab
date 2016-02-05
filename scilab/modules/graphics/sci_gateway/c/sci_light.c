/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Pedro SOUZA
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

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "BuildObjects.h"
#include "os_string.h"

static const char fname[] = "light";

int sci_light(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)
{
    int light = 0;
    if (nin > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname, 0, 1);
        return 1;
    }

    if (nin + nopt == 0)
    {
        light = ConstructLight(fname, 0, -1, TRUE, NULL, NULL, NULL, NULL, NULL);
    }
    else
    {
        int type = -1;
        BOOL visible = 1;
        double* position = NULL;
        double* direction = NULL;
        double* ambient_color = NULL;
        double* diffuse_color = NULL;
        double* specular_color = NULL;
        int axes = 0;
        scilabVar var = NULL;

        if (nin > 0 && scilab_isHandle(env, in[0]))
        {
            long long axesHandle = 0;
            if (scilab_isScalar(env, in[0]))
            {
                if (scilab_getHandle(env, in[0], &axesHandle))
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle expected.\n"), fname, 1);
                    return 1;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return 1;
            }

            axes = getObjectFromHandle((long)axesHandle);
        }

        //optionals
        var = scilab_getOptional(env, opt, L"visible");
        if (var && scilab_isString(env, var) && scilab_isScalar(env, var))
        {
            wchar_t* wstr = NULL;
            if (scilab_getString(env, var, &wstr))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 7);
                return 0;
            }

            if (wcsicmp(wstr, L"on") == 0)
            {
                visible = 1;
            }
            else if (wcsicmp(wstr, L"off") == 0)
            {
                visible = 0;
            }
        }

        var = scilab_getOptional(env, opt, L"type");
        if (var && scilab_isString(env, var) && scilab_isScalar(env, var))
        {
            wchar_t* wstr = NULL;
            if (scilab_getString(env, var, &wstr))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 7);
                return 0;
            }

            if (wcsicmp(wstr, L"directional") == 0)
            {
                type = 0;
            }
            else if (wcsicmp(wstr, L"point") == 0)
            {
                type = 1;
            }
        }

        var = scilab_getOptional(env, opt, L"position");
        if (var && scilab_isDouble(env, var) && scilab_getSize(env, var) == 3)
        {
            scilab_getDoubleArray(env, var, &position);
        }

        var = scilab_getOptional(env, opt, L"direction");
        if (var && scilab_isDouble(env, var) && scilab_getSize(env, var) == 3)
        {
            scilab_getDoubleArray(env, var, &direction);
        }

        var = scilab_getOptional(env, opt, L"ambient_color");
        if (var && scilab_isDouble(env, var) && scilab_getSize(env, var) == 3)
        {
            scilab_getDoubleArray(env, var, &ambient_color);
        }

        var = scilab_getOptional(env, opt, L"diffuse_color");
        if (var && scilab_isDouble(env, var) && scilab_getSize(env, var) == 3)
        {
            scilab_getDoubleArray(env, var, &diffuse_color);
        }

        var = scilab_getOptional(env, opt, L"specular_color");
        if (var && scilab_isDouble(env, var) && scilab_getSize(env, var) == 3)
        {
            scilab_getDoubleArray(env, var, &specular_color);
        }

        light = ConstructLight(fname, axes, type, visible, position, direction, ambient_color, diffuse_color, specular_color);
    }

    //error occurs in ConstructLight
    if (light == 0)
    {
        //error is manage in ConstructLight
        return 1;
    }

    out[0] = scilab_createHandle(env);
    if (out[0] == NULL)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    scilab_setHandle(env, out[0], getHandle(light));
    return 0;
}
