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

extern "C"
{
#include <stdlib.h>
#include "gw_gui.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "localization.h"
#include "GiwsException.hxx"
}

/*--------------------------------------------------------------------------*/
int sci_getinstalledlookandfeels(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 1, 1);

    org_scilab_modules_gui_utils::LookAndFeelManager * lnf = 0;
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
        char **lookandfeels = NULL;
        int nbElems = 0;
        int nbCol = 0;

        lookandfeels = lnf->getInstalledLookAndFeels();
        nbElems = lnf->numbersOfInstalledLookAndFeels();

        nbCol = 1;

        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbElems, nbCol, lookandfeels);

        if (lookandfeels)
        {
            for (int i = 0; i < nbElems; i++)
            {
                if (lookandfeels[i])
                {
                    delete[] lookandfeels[i];
                }
            }
            delete[] lookandfeels;
            lookandfeels = NULL;
        }
        delete lnf;

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
