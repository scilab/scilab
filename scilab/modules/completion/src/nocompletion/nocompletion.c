/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "completion.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_completion.h"
/*--------------------------------------------------------------------------*/
int gw_completion(void)
{
    Scierror(999, _("Scilab completion module not installed.\n"));
    return 0;
}
/*--------------------------------------------------------------------------*/
