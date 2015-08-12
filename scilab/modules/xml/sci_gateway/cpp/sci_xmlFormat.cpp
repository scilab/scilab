/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include "gw_xml.h"
#include "api_scilab.h"
}

/*--------------------------------------------------------------------------*/
int sci_xmlFormat(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 1);
    CheckLhs(1, 1);

    OverLoad(1);

    return 0;
}
