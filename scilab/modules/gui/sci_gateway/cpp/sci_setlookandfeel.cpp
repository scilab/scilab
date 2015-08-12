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
#include "GiwsException.hxx"

extern "C"
{
#include "gw_gui.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "localization.h"
#include "BOOL.h"
#include "MALLOC.h"
    /*--------------------------------------------------------------------------*/
    int sci_setlookandfeel(char *fname, unsigned long fname_len)
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
                returnArguments(pvApiCtx);
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
                returnArguments(pvApiCtx);
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
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return 1;
        }

        return 0;
    }
    /*--------------------------------------------------------------------------*/

}

/* END OF extern "C" */
/*--------------------------------------------------------------------------*/
