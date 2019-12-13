/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2019 - ESI Group - Antoine ELIAS
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

#include <unistd.h>
#include "time_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

static char fname[] = "sleep";

types::Function::ReturnValue sci_sleep(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRhs = static_cast<int>(in.size());
    bool asSec = false;
    double t = 0;

    if (iRhs < 1 || iRhs > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname, 1, 2);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 1);
        return types::Function::Error;
    }

    t = in[0]->getAs<types::Double>()->get()[0];
    if (t < 0)
    {
        Scierror(999, _( "%s: Argument #%d: the scalar must be positive.\n"), fname, 1);
        return types::Function::Error;
    }

    if (iRhs == 2)
    {
        if (in[1]->isString() == false ||
            in[1]->getAs<types::String>()->isScalar() == false ||
            wcscmp(in[1]->getAs<types::String>()->get()[0], L"s") != 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 's' expected.\n"), fname, 2);
            return types::Function::Error;
        }

        asSec = true;
    }

#ifdef _MSC_VER
    {
        if (asSec)
        {
            t *= 1000; /* convert seconds into milliseconds */
        }

        if (t)
        {
            Sleep((DWORD)t);    /* Number of milliseconds to sleep. */
        }
    }
#else
    {
        if (asSec)
        {
            t *= 1000;
        }

        if (t)
#ifdef HAVE_USLEEP
        {
            usleep(t * 1000);
        }
#else
#ifdef HAVE_SLEEP
        {
            sleep(t * 1000);
        }
#endif
#endif
    }
#endif

    return types::Function::OK;
}

