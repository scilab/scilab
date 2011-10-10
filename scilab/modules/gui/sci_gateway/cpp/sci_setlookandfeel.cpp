/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "LookAndFeelManager.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_gui.h"
#include "stack-c.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "localization.h"
#include "BOOL.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
    int sci_setlookandfeel(char *fname, unsigned long fname_len)
    {
        CheckRhs(0, 1);
        CheckLhs(0, 1);

        org_scilab_modules_gui_utils::LookAndFeelManager * lnf = 0;

        if (Rhs == 0)
        {
            try
            {
                lnf = new org_scilab_modules_gui_utils::LookAndFeelManager(getScilabJavaVM());
            }
            catch(const GiwsException::JniException & e)
            {
                Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
                return FALSE;
            }

            if (lnf)
            {
                int n1 = 1;
                int *paramoutINT = (int *)MALLOC(sizeof(int));

                *paramoutINT = (int)booltoBOOL(lnf->setSystemLookAndFeel());
                delete lnf;

                n1 = 1;
                CreateVarFromPtr(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &paramoutINT);
                LhsVar(1) = Rhs + 1;

                if (paramoutINT)
                {
                    FREE(paramoutINT);
                    paramoutINT = NULL;
                }
                PutLhsVar();
            }
            else
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
            }
        }
        else if (GetType(1) == sci_strings)
        {
            int m1, n1 = 0, l1 = 0;

            char *looknfeel = NULL;

            GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
            looknfeel = cstk(l1);

            try
            {
                lnf = new org_scilab_modules_gui_utils::LookAndFeelManager(getScilabJavaVM());
            }
            catch(const GiwsException::JniException & e)
            {
                Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
                return FALSE;
            }

            if (lnf)
            {
                int *paramoutINT = (int *)MALLOC(sizeof(int));

                *paramoutINT = (int)booltoBOOL(lnf->setLookAndFeel(looknfeel));
                delete lnf;

                n1 = 1;
                CreateVarFromPtr(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &paramoutINT);
                LhsVar(1) = Rhs + 1;

                if (paramoutINT)
                {
                    FREE(paramoutINT);
                    paramoutINT = NULL;
                }
                PutLhsVar();
            }
            else
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        }
        return 0;
    }
/*--------------------------------------------------------------------------*/

}

/* END OF extern "C" */
/*--------------------------------------------------------------------------*/
