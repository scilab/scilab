/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
    /*--------------------------------------------------------------------------*/
    int sci_getinstalledlookandfeels(char *fname, unsigned long fname_len)
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
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            if (lookandfeels)
            {
                int i = 0;

                for (i = 0; i < nbElems; i++)
                {
                    if (lookandfeels[i])
                    {
                        delete[]lookandfeels[i];
                    }
                }
                delete[]lookandfeels;
                lookandfeels = NULL;
            }
            delete lnf;

            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            returnArguments(pvApiCtx);
        }
        else
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
        }
        return 0;
    }
    /*--------------------------------------------------------------------------*/
}

/* END OF extern "C" */
/*--------------------------------------------------------------------------*/
