/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
/*------------------------------------------------------------------------*/
/* desc : interface for tohome primitive                                  */
/*------------------------------------------------------------------------*/
#include "gw_console.h"
#include "api_scilab.h"
#include "localization.h"
#include "tohome.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
int sci_tohome(char *fname, void* pvApiCtx)
{

    CheckRhs(0, 0);
    CheckLhs(0, 1);

    if (!tohome())
    {
        sciprint(_("%s: This feature has not been implemented in this mode.\n"), fname);
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0 ;
}
/*--------------------------------------------------------------------------*/
