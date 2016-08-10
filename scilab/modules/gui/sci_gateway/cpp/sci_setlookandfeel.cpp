/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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

#include "LookAndFeelManager.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_gui.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "localization.h"
#include "BOOL.h"
#include "sci_malloc.h"
}
/*--------------------------------------------------------------------------*/
int sci_setlookandfeel(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    org_scilab_modules_gui_utils::LookAndFeelManager * lnf = 0;

    if (nbInputArgument(pvApiCtx) == 0)
    {
        try
        {
            lnf = new org_scilab_modules_gui_utils::LookAndFeelManager(getScilabJavaVM());
        }
        catch (const GiwsException::JniException & e)
        {
            Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
            return 1;
        }

        if (lnf)
        {
            int paramoutINT = 0;

            paramoutINT = (int)booltoBOOL(lnf->setSystemLookAndFeel());
            delete lnf;

            if (createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, paramoutINT))
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            ReturnArguments(pvApiCtx);
            return 0;
        }
        else
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 1;
        }
    }
    else if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
    {
        char *looknfeel = NULL;
        int* piAddrStr = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrStr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddrStr, &looknfeel))
        {
            printError(&sciErr, 0);
            return 1;
        }

        try
        {
            lnf = new org_scilab_modules_gui_utils::LookAndFeelManager(getScilabJavaVM());
        }
        catch (const GiwsException::JniException & e)
        {
            freeAllocatedSingleString(looknfeel);
            Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
            return 1;
        }

        if (lnf)
        {
            int paramoutINT = 0;

            paramoutINT = (int)booltoBOOL(lnf->setLookAndFeel(looknfeel));
            freeAllocatedSingleString(looknfeel);
            delete lnf;

            if (createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, paramoutINT))
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            ReturnArguments(pvApiCtx);
            return 0;
        }
        else
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
