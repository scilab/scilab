/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2010 - Clément DAVID
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
/*--------------------------------------------------------------------------*/
#include "SciNotes.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_scinotes.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_scinotes;

/*--------------------------------------------------------------------------*/
int sci_closeSciNotesFromScilab(char * fname, void* pvApiCtx)
{
    CheckRhs(0, 0);
    CheckLhs(0, 1);

    try
    {
        JavaVM *jvm = getScilabJavaVM();

        if (jvm)
        {
            SciNotes::closeSciNotesFromScilab(jvm);
        }
    }
    catch (GiwsException::JniCallMethodException exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
        return 0;
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
