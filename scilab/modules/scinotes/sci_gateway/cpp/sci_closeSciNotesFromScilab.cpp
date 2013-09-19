/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2010 - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
int sci_closeSciNotesFromScilab(char *fname, unsigned long fname_len)
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
