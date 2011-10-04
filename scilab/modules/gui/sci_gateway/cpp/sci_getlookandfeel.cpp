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

extern "C"
{
#include <stdlib.h>
#include "gw_gui.h"
#include "stack-c.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "localization.h"
#include "GiwsException.hxx"
/*--------------------------------------------------------------------------*/
    int sci_getlookandfeel(char *fname, unsigned long fname_len)
    {
        CheckRhs(0, 0);
        CheckLhs(1, 1);

        org_scilab_modules_gui_utils::LookAndFeelManager * lnf = 0;
        try
        {
            lnf = new org_scilab_modules_gui_utils::LookAndFeelManager(getScilabJavaVM());
        }
        catch(const GiwsException::JniException & e)
        {
            Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
            return 0;
        }

        if (lnf)
        {
            static int n1 = 0, m1 = 0;

            char *look = lnf->getCurrentLookAndFeel();

            if (look)
            {
                m1 = (int)strlen(look);
                n1 = 1;
                CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, &m1, &n1, &look);
                if (look)
                {
                    delete[]look;
                    look = NULL;
                }
                delete lnf;

                LhsVar(1) = Rhs + 1;
                PutLhsVar();
            }
            else
            {
                delete lnf;

                Scierror(999, _("%s: An error occurred: %s.\n"), fname, _("Impossible to get current look and feel"));
            }
        }
        else
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
        }

        return 0;
    }
/*--------------------------------------------------------------------------*/

}                               /* END OF extern "C" */

/*--------------------------------------------------------------------------*/
