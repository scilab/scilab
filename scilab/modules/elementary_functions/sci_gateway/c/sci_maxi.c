/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_elementary_functions.h"
#include "warningmode.h"
#include "localization.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intmaxi)(char *fname,int *id,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int sci_maxi(char *fname, unsigned long fname_len)
{
	static int id[6];

    if ((strcmp(fname, "maxi") == 0) | (strcmp(fname, "mini") == 0))
    {
        if (getWarningMode())
        {
            sciprint(_("Warning: "));
            sciprint(_("Function %s is obsolete."), fname);
            sciprint("\n");
            sciprint(_("Warning: "));
            if (strcmp(fname, "maxi") == 0)
            {
                sciprint(_("Please use %s instead."), "max");
            }
            else
            {
                sciprint(_("Please use %s instead."), "min");
            }
            sciprint("\n");
            sciprint(_("Warning: "));
            sciprint(_("This function will be permanently removed in Scilab %s"), "5.3.1");
            sciprint("\n");
        }
    }

	C2F(intmaxi)(fname, id, fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
